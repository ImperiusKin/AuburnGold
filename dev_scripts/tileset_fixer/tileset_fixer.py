#!/usr/bin/env python3
"""
Per-tileset maintenance tool.

Run interactively:
    python dev_scripts/tileset_fixer/tileset_fixer.py

Or target a tileset directly:
    python dev_scripts/tileset_fixer/tileset_fixer.py --tileset gTileset_Pokemon_Center --wipe
    python dev_scripts/tileset_fixer/tileset_fixer.py --tileset gTileset_Pokemon_Center --dedupe-tiles

Wipe touches only metatile_attributes.bin (behavior / terrain type / encounter
type / layer type). Dedupe touches only tiles.png and metatiles.bin (tile
graphics + which tile/flip/palette each metatile slot points at).
"""
import argparse
import json
import re
import struct
import sys
from pathlib import Path

import png_indexed as png_util

ROOT = Path(__file__).resolve().parents[2]
HEADERS_H = ROOT / "src/data/tilesets/headers.h"
METATILES_H = ROOT / "src/data/tilesets/metatiles.h"
GRAPHICS_H = ROOT / "src/data/tilesets/graphics.h"
LAYOUTS_JSON = ROOT / "data/layouts/layouts.json"
MAPS_DIR = ROOT / "data/maps"

TILES_INCBIN_RE = re.compile(r"const u32 (\w+)\[\]\s*=\s*INCBIN_U32\(\"([^\"]+)\"\)")
PALETTES_BLOCK_RE = re.compile(r"const u16 (\w+)\[\]\[16\]\s*=\s*\{(.*?)\};", re.DOTALL)
PALETTE_INCBIN_RE = re.compile(r"INCBIN_U16\(\"([^\"]+)\"\)")

NUM_TILES_IN_PRIMARY = {"emerald": 512, "frlg": 640}
NUM_TILES_TOTAL = 1024
TILE_ID_MASK = 0x3FF
TILE_HFLIP_BIT = 0x400
TILE_VFLIP_BIT = 0x800

# Triple-layer metatiles (see wiki: Triple-layer-metatiles): each metatile is
# 12 tile-slots (bottom/middle/top, 4 each) * 2 bytes = 24 bytes in
# metatiles.bin, instead of the vanilla 8 tile-slots / 16 bytes.
NUM_TILES_PER_METATILE = 12
METATILE_BYTES = NUM_TILES_PER_METATILE * 2

# Expanded metatile count (see wiki: Expanding-The-Metatile-Count): map grid
# words (data/layouts/*/map.bin and border.bin) now pack a 12-bit metatile id
# instead of 10-bit, so the primary/secondary boundary within THAT id space
# is NUM_METATILES_IN_PRIMARY (2048/2560), NOT NUM_TILES_IN_PRIMARY
# (512/640, which is a different, still-10-bit boundary for tile-GRAPHICS
# references inside metatiles.bin content - those two coincided numerically
# before this expansion but no longer do). Mixing them up silently
# misclassifies which tileset a placed metatile ID belongs to.
NUM_METATILES_IN_PRIMARY = {"emerald": 2048, "frlg": 2560}
NUM_METATILES_TOTAL_EXPANDED = 4096
MAPGRID_METATILE_ID_MASK = 0x0FFF

# Palette banks: NOT a clean 6+6 split. Confirmed from src/fieldmap.c
# (LoadTilesetPalette / LoadSecondaryTilesetPalette): the secondary tileset's
# palette array is read starting at array index NUM_PALS_IN_PRIMARY (not 0!),
# and the array index IS the global bank number for both tilesets - there is
# no "-6" local offset for secondary. So a tileset's palette_paths[N] always
# holds bank N's data directly, whether that tileset is primary or secondary.
# Primary owns banks [0, NUM_PALS_IN_PRIMARY); secondary owns
# [NUM_PALS_IN_PRIMARY, NUM_PALS_TOTAL). Array slots below a secondary's own
# starting bank (e.g. files 00-05.pal for emerald) are never read by the
# engine at all.
NUM_PALS_IN_PRIMARY = {"emerald": 6, "frlg": 7}
NUM_PALS_TOTAL = 13

TILESET_BLOCK_RE = re.compile(
    r"const struct Tileset (gTileset_\w+)\s*=\s*\{(.*?)\};", re.DOTALL
)
FIELD_RE = re.compile(r"\.(\w+)\s*=\s*([^,\n]+),?")
INCBIN_RE = re.compile(r"const u16 (\w+)\[\]\s*=\s*INCBIN_U16\(\"([^\"]+)\"\)")


class Tileset:
    def __init__(self, name, is_secondary, metatiles_sym, attrs_sym, is_compressed=True,
                 tiles_sym="", palettes_sym=""):
        self.name = name
        self.is_secondary = is_secondary
        self.metatiles_sym = metatiles_sym
        self.attrs_sym = attrs_sym
        self.is_compressed = is_compressed
        self.tiles_sym = tiles_sym
        self.palettes_sym = palettes_sym
        self.metatiles_path = None
        self.attrs_path = None
        self.tiles_path = None       # resolved from graphics.h
        self.palette_paths = []      # resolved from graphics.h, in order
        self.maps = []  # list of (map_name, role) role = "primary" | "secondary"

    @property
    def dir(self):
        return self.attrs_path.parent if self.attrs_path else None


def parse_incbin_paths():
    """Map symbol name -> path, for both metatiles.bin and metatile_attributes.bin arrays."""
    paths = {}
    text = METATILES_H.read_text()
    for sym, path in INCBIN_RE.findall(text):
        paths[sym] = ROOT / path
    return paths


def parse_graphics_paths():
    """From graphics.h: tiles symbol -> path, and palettes symbol -> ordered list of .gbapal paths."""
    text = GRAPHICS_H.read_text()
    tiles_paths = {sym: ROOT / path for sym, path in TILES_INCBIN_RE.findall(text)}
    palette_paths = {}
    for sym, body in PALETTES_BLOCK_RE.findall(text):
        palette_paths[sym] = [ROOT / p for p in PALETTE_INCBIN_RE.findall(body)]
    return tiles_paths, palette_paths


def parse_tilesets(incbin_paths, tiles_paths, palette_paths):
    tilesets = {}
    text = HEADERS_H.read_text()
    for name, body in TILESET_BLOCK_RE.findall(text):
        fields = dict(FIELD_RE.findall(body))
        is_secondary = fields.get("isSecondary", "FALSE").strip() == "TRUE"
        is_compressed = fields.get("isCompressed", "FALSE").strip() == "TRUE"
        metatiles_sym = fields.get("metatiles", "").strip()
        attrs_sym = fields.get("metatileAttributes", "").strip()
        tiles_sym = fields.get("tiles", "").strip()
        palettes_sym = fields.get("palettes", "").strip()
        ts = Tileset(name, is_secondary, metatiles_sym, attrs_sym, is_compressed,
                      tiles_sym, palettes_sym)
        ts.metatiles_path = incbin_paths.get(metatiles_sym)
        ts.attrs_path = incbin_paths.get(attrs_sym)
        ts.tiles_path = tiles_paths.get(tiles_sym)
        ts.palette_paths = palette_paths.get(palettes_sym, [])
        if ts.metatiles_path and ts.attrs_path:
            tilesets[name] = ts
    return tilesets


def attach_map_usage(tilesets):
    layouts = json.loads(LAYOUTS_JSON.read_text())["layouts"]
    # layout id -> [(tileset_name, role), ...]
    layout_tileset_roles = {}
    for layout in layouts:
        roles = []
        if layout.get("primary_tileset"):
            roles.append((layout["primary_tileset"], "primary"))
        if layout.get("secondary_tileset"):
            roles.append((layout["secondary_tileset"], "secondary"))
        layout_tileset_roles[layout["id"]] = roles

    for map_json in MAPS_DIR.glob("*/map.json"):
        data = json.loads(map_json.read_text())
        layout_id = data.get("layout")
        map_name = data.get("name", map_json.parent.name)
        for tileset_name, role in layout_tileset_roles.get(layout_id, []):
            ts = tilesets.get(tileset_name)
            if ts:
                ts.maps.append((map_name, role))


def _prompt_restore(what, ask_restore=True):
    """Returns True if the caller should undo what it just did."""
    if not ask_restore:
        return False
    ans = input(f"Restore original {what}? [y/N]: ").strip().lower()
    return ans in ("y", "yes")


def find_used_local_metatile_indices(ts):
    """Local metatile indices (into ts's own metatiles.bin/attributes) that are
    actually placed on some map or border, across every layout using ts in
    either role. A metatile ID not in this set is placed nowhere in the game."""
    fmt, _ = detect_format(ts)
    metatile_boundary = NUM_METATILES_IN_PRIMARY[fmt]
    local_offset = metatile_boundary if ts.is_secondary else 0
    local_hi = NUM_METATILES_TOTAL_EXPANDED if ts.is_secondary else metatile_boundary

    layouts = json.loads(LAYOUTS_JSON.read_text())["layouts"]
    used = set()
    for layout in layouts:
        if layout.get("primary_tileset") != ts.name and layout.get("secondary_tileset") != ts.name:
            continue
        for filekey in ("blockdata_filepath", "border_filepath"):
            rel = layout.get(filekey)
            if not rel:
                continue
            path = ROOT / rel
            if not path.exists():
                continue
            data = path.read_bytes()
            n = len(data) // 2
            for v in struct.unpack(f"<{n}H", data):
                mid = v & MAPGRID_METATILE_ID_MASK
                if local_offset <= mid < local_hi:
                    used.add(mid - local_offset)
    return used


def detect_format(ts):
    """Return ('emerald', 2) or ('frlg', 4) bytes-per-attribute-entry, based on
    the ratio between metatile_attributes.bin and metatiles.bin sizes (each
    metatile is NUM_TILES_PER_METATILE tiles * 2 bytes = METATILE_BYTES bytes
    in metatiles.bin, under the triple-layer metatile system)."""
    metatiles_size = ts.metatiles_path.stat().st_size
    attrs_size = ts.attrs_path.stat().st_size
    num_metatiles = metatiles_size // METATILE_BYTES
    if num_metatiles == 0:
        return None, None
    bytes_per_entry = attrs_size / num_metatiles
    if bytes_per_entry == 2:
        return "emerald", 2
    if bytes_per_entry == 4:
        return "frlg", 4
    return "unknown", None


def wipe_attributes(ts, ask_restore=True):
    """Zero every metatile_attributes.bin entry. Value 0 decodes to
    MB_NORMAL behavior, TILE_TERRAIN_NORMAL, TILE_ENCOUNTER_NONE and
    METATILE_LAYER_TYPE_NORMAL simultaneously in both formats, since all
    four are enum value 0."""
    fmt, entry_size = detect_format(ts)
    if entry_size is None:
        print(f"  ! Could not determine attribute format for {ts.name} "
              f"(metatiles.bin size doesn't divide evenly by {METATILE_BYTES}). Aborting.")
        return False

    backup = ts.attrs_path.with_suffix(ts.attrs_path.suffix + ".bak")
    if not backup.exists():
        backup.write_bytes(ts.attrs_path.read_bytes())
        print(f"  Backed up original to {backup.relative_to(ROOT)}")
    else:
        print(f"  Backup already exists at {backup.relative_to(ROOT)} (not overwritten)")

    size = ts.attrs_path.stat().st_size
    ts.attrs_path.write_bytes(b"\x00" * size)
    print(f"  Wiped {size} bytes ({size // entry_size} metatiles, {fmt} format) "
          f"in {ts.attrs_path.relative_to(ROOT)}")

    if _prompt_restore("metatile_attributes.bin", ask_restore):
        ts.attrs_path.write_bytes(backup.read_bytes())
        print("  Restored original metatile_attributes.bin.")
    return True


def _tile_pixels(png_img, tile_index, num_cols):
    col = tile_index % num_cols
    row = tile_index // num_cols
    ox, oy = col * 8, row * 8
    tile = bytearray(64)
    for dy in range(8):
        base = oy + dy
        for dx in range(8):
            tile[dy * 8 + dx] = png_img.get(ox + dx, base)
    return bytes(tile)


def _xflip(tile):
    out = bytearray(64)
    for y in range(8):
        row = tile[y * 8:y * 8 + 8]
        out[y * 8:y * 8 + 8] = row[::-1]
    return bytes(out)


def _yflip(tile):
    out = bytearray(64)
    for y in range(8):
        out[y * 8:y * 8 + 8] = tile[(7 - y) * 8:(8 - y) * 8]
    return bytes(out)


def _dedupe_tiles(tiles):
    """Returns (unique_tiles, mapping) where mapping[old_index] = (new_index, flip_x, flip_y):
    to reconstruct tile `old_index`, take unique_tiles[new_index] and apply flip_x/flip_y."""
    seen = {}
    unique = []
    mapping = {}
    for i, t in enumerate(tiles):
        if t in seen:
            mapping[i] = (seen[t], False, False)
            continue
        xf = _xflip(t)
        if xf in seen:
            mapping[i] = (seen[xf], True, False)
            continue
        yf = _yflip(t)
        if yf in seen:
            mapping[i] = (seen[yf], False, True)
            continue
        xyf = _xflip(yf)
        if xyf in seen:
            mapping[i] = (seen[xyf], True, True)
            continue
        new_idx = len(unique)
        unique.append(t)
        seen[t] = new_idx
        mapping[i] = (new_idx, False, False)
    return unique, mapping


def dedupe_tileset(ts, ask_restore=True):
    fmt, _ = detect_format(ts)
    if fmt not in ("emerald", "frlg"):
        print(f"  ! Could not determine format for {ts.name}. Aborting.")
        return False

    tiles_png_path = ts.dir / "tiles.png"
    if not tiles_png_path.exists():
        print(f"  ! No tiles.png found at {tiles_png_path.relative_to(ROOT)}. Aborting.")
        return False

    if (ts.dir / "anim").is_dir():
        print(f"  Note: {ts.name} has an anim/ subfolder. Animated tile slots are "
              f"NOT protected by this tool and may end up pointing at the wrong "
              f"graphic after dedupe.")

    img = png_util.read_indexed_png(tiles_png_path)
    num_cols = img.width // 8
    num_tiles = (img.height // 8) * num_cols
    tiles = [_tile_pixels(img, i, num_cols) for i in range(num_tiles)]

    unique, mapping = _dedupe_tiles(tiles)
    removed = len(tiles) - len(unique)
    if removed == 0:
        print(f"  No duplicate tiles found in {tiles_png_path.relative_to(ROOT)} "
              f"({len(tiles)} tiles).")
        return True

    print(f"  {len(tiles)} tiles -> {len(unique)} unique ({removed} removed)")

    # Rebuild tiles.png with only unique tiles, padding the last row with blank tiles.
    new_rows = (len(unique) + num_cols - 1) // num_cols
    new_height = new_rows * 8
    new_pixels = bytearray(img.width * new_height)
    for idx, tile in enumerate(unique):
        col, row = idx % num_cols, idx // num_cols
        ox, oy = col * 8, row * 8
        for dy in range(8):
            new_pixels[(oy + dy) * img.width + ox: (oy + dy) * img.width + ox + 8] = \
                tile[dy * 8: dy * 8 + 8]
    new_img = png_util.IndexedPng(img.width, new_height, img.bit_depth, new_pixels, img.chunks)

    png_backup = tiles_png_path.with_suffix(tiles_png_path.suffix + ".bak")
    if not png_backup.exists():
        png_backup.write_bytes(tiles_png_path.read_bytes())
        print(f"  Backed up {png_backup.relative_to(ROOT)}")
    png_util.write_indexed_png(tiles_png_path, new_img)
    print(f"  Wrote {tiles_png_path.relative_to(ROOT)} ({new_rows} rows, {len(unique)} tiles, "
          f"{num_cols * new_rows - len(unique)} blank padding slots)")

    # Update metatiles.bin: only remap tile ids that fall within this tileset's
    # own local range, so cross-references to the paired tileset are left alone.
    local_offset = NUM_TILES_IN_PRIMARY[fmt] if ts.is_secondary else 0
    local_hi = NUM_TILES_TOTAL if ts.is_secondary else NUM_TILES_IN_PRIMARY[fmt]

    data = ts.metatiles_path.read_bytes()
    n = len(data) // 2
    entries = list(struct.unpack(f"<{n}H", data))
    changed = 0
    for i, v in enumerate(entries):
        tile_id = v & TILE_ID_MASK
        if not (local_offset <= tile_id < local_hi):
            continue  # cross-tileset reference (or out of range) - leave untouched
        local_idx = tile_id - local_offset
        if local_idx not in mapping:
            continue
        new_local, fx, fy = mapping[local_idx]
        if new_local == local_idx and not fx and not fy:
            continue
        hflip = bool(v & TILE_HFLIP_BIT) ^ fx
        vflip = bool(v & TILE_VFLIP_BIT) ^ fy
        pal_bits = v & ~(TILE_ID_MASK | TILE_HFLIP_BIT | TILE_VFLIP_BIT)
        new_v = pal_bits | ((new_local + local_offset) & TILE_ID_MASK)
        if hflip:
            new_v |= TILE_HFLIP_BIT
        if vflip:
            new_v |= TILE_VFLIP_BIT
        entries[i] = new_v
        changed += 1

    metatiles_backup = ts.metatiles_path.with_suffix(ts.metatiles_path.suffix + ".bak")
    if not metatiles_backup.exists():
        metatiles_backup.write_bytes(data)
        print(f"  Backed up {metatiles_backup.relative_to(ROOT)}")
    ts.metatiles_path.write_bytes(struct.pack(f"<{n}H", *entries))
    print(f"  Updated {changed} metatile tile-slot reference(s) in "
          f"{ts.metatiles_path.relative_to(ROOT)}")

    if _prompt_restore("tiles.png and metatiles.bin", ask_restore):
        tiles_png_path.write_bytes(png_backup.read_bytes())
        ts.metatiles_path.write_bytes(metatiles_backup.read_bytes())
        print("  Restored original tiles.png and metatiles.bin.")
    return True


def read_jasc_pal(path):
    """JASC-PAL text format: header, version, count, then 'R G B' lines."""
    lines = path.read_text().splitlines()
    count = int(lines[2])
    colors = []
    for line in lines[3:3 + count]:
        r, g, b = (int(x) for x in line.split())
        colors.append((r, g, b))
    return colors


def _used_metatile_ids_for_role(ts, role):
    """Every metatile ID (global 0-1023 numbering) placed on a map or border,
    across EVERY layout that uses ts in the given role - regardless of what
    it's paired with. A primary tileset's banks can be shared by more than one
    secondary (e.g. Basic_Interior is paired with both Startingtowninterior and
    Basement), so this must not be scoped to just one specific pairing, or a
    palette repack could silently break maps that use the other pairing."""
    key = "primary_tileset" if role == "primary" else "secondary_tileset"
    layouts = json.loads(LAYOUTS_JSON.read_text())["layouts"]
    ids = set()
    for layout in layouts:
        if layout.get(key) != ts.name:
            continue
        for filekey in ("blockdata_filepath", "border_filepath"):
            rel = layout.get(filekey)
            if not rel:
                continue
            path = ROOT / rel
            if not path.exists():
                continue
            data = path.read_bytes()
            n = len(data) // 2
            for v in struct.unpack(f"<{n}H", data):
                ids.add(v & MAPGRID_METATILE_ID_MASK)
    return ids


PALETTE_BANK_CAPACITY = 15  # 16 slots minus the shared, content-agnostic transparent slot 0


def _bank_colors_for_ids(fmt, primary_ts, secondary_ts, used_ids, metatile_boundary, tile_local_offset,
                          prim_img, prim_cols, sec_img, sec_cols, prim_entries, sec_entries):
    """bank -> {color_index: (r,g,b)} for every bank referenced by tile-slots of
    the given (already role-scoped) set of placed metatile IDs. Index 0 (the
    transparent slot) is excluded from every bank's set - it never needs to
    match/coexist with anything.

    Two different boundaries are in play here, and they are NOT the same
    number post metatile-count-expansion: `metatile_boundary` splits `mid`
    (a metatile ID, in the 12-bit map-grid ID space, NUM_METATILES_IN_PRIMARY)
    while `tile_local_offset` splits `tile_id` (a tile-GRAPHICS reference
    embedded in a metatile's own tile-slot word, still the original 10-bit
    NUM_TILES_IN_PRIMARY space - metatiles.bin content format didn't change)."""
    num_pals_primary = NUM_PALS_IN_PRIMARY[fmt]
    bank_indices_used = {b: set() for b in range(NUM_PALS_TOTAL)}
    for mid in used_ids:
        if mid < metatile_boundary:
            entries, local_idx = prim_entries, mid
        else:
            entries, local_idx = sec_entries, mid - metatile_boundary
        for slot in range(NUM_TILES_PER_METATILE):
            pos = local_idx * NUM_TILES_PER_METATILE + slot
            if pos >= len(entries):
                continue
            v = entries[pos]
            bank = (v & 0xF000) >> 12
            tile_id = v & TILE_ID_MASK
            if bank >= NUM_PALS_TOTAL:
                continue  # not a map-tileset palette bank
            if tile_id < tile_local_offset:
                pixels = _tile_pixels(prim_img, tile_id, prim_cols)
            else:
                pixels = _tile_pixels(sec_img, tile_id - tile_local_offset, sec_cols)
            bank_indices_used[bank].update(p for p in pixels if p != 0)

    def bank_source(bank):
        # Array index == global bank number for BOTH tilesets - confirmed from
        # LoadTilesetPalette in src/fieldmap.c. No "-6" offset for secondary.
        return (primary_ts, bank) if bank < num_pals_primary else (secondary_ts, bank)

    bank_colors = {}
    for bank, indices in bank_indices_used.items():
        if not indices:
            continue
        ts, array_idx = bank_source(bank)
        if array_idx >= len(ts.palette_paths):
            continue
        pal = read_jasc_pal(ts.palette_paths[array_idx].with_suffix(".pal"))
        bank_colors[bank] = {i: pal[i] for i in indices if i < len(pal)}
    return bank_colors


def _bin_pack_banks(bank_colors, capacity=PALETTE_BANK_CAPACITY):
    """Greedy first-fit-decreasing: merge banks whose colors can coexist within
    one palette, minimizing total banks needed. Returns a list of groups, each
    a dict of {(r,g,b): (source_bank, source_index)}."""
    used_banks = sorted(bank_colors, key=lambda b: -len(bank_colors[b]))
    groups = []
    for bank in used_banks:
        colors = bank_colors[bank]
        placed = False
        for group in groups:
            new_colors = {c for c in colors.values() if c not in group}
            if len(group) + len(new_colors) <= capacity:
                for idx, c in colors.items():
                    group.setdefault(c, (bank, idx))
                placed = True
                break
        if not placed:
            groups.append({c: (bank, idx) for idx, c in colors.items()})
    return groups


def _bin_pack_banks_index_preserving(bank_colors, capacity=PALETTE_BANK_CAPACITY):
    """Like _bin_pack_banks, but for when tile pixel data will NOT be rewritten:
    a color must stay at its original index within whatever bank ends up
    hosting it (pixel value 5 always means 'slot 5 of my assigned bank'), so
    two banks can only merge if their used INDEX SETS are disjoint - matching
    color counts isn't enough, since two different colors at the same index
    from two different source banks can't coexist without moving one of them.
    Returns a list of groups: {index: (rgb, source_bank)}."""
    used_banks = sorted(bank_colors, key=lambda b: -len(bank_colors[b]))
    groups = []
    for bank in used_banks:
        colors = bank_colors[bank]  # {index: rgb}
        idxset = set(colors)
        placed = False
        for group in groups:
            if idxset & group.keys():
                continue  # index collision - can't coexist without rewriting pixel data
            if len(group) + len(idxset) <= capacity:
                for idx, c in colors.items():
                    group[idx] = (c, bank)
                placed = True
                break
        if not placed:
            groups.append({idx: (c, bank) for idx, c in colors.items()})
    return groups


def _load_tileset_pair(primary_ts, secondary_ts):
    """Returns (metatile_boundary, tile_local_offset, prim_img, prim_cols,
    sec_img, sec_cols, prim_entries, sec_entries). See _bank_colors_for_ids
    for why these are two distinct boundary values now."""
    fmt, _ = detect_format(secondary_ts)
    metatile_boundary = NUM_METATILES_IN_PRIMARY[fmt]
    tile_local_offset = NUM_TILES_IN_PRIMARY[fmt]
    prim_img = png_util.read_indexed_png(primary_ts.dir / "tiles.png")
    sec_img = png_util.read_indexed_png(secondary_ts.dir / "tiles.png")
    prim_entries = struct.unpack(
        f"<{primary_ts.metatiles_path.stat().st_size // 2}H", primary_ts.metatiles_path.read_bytes())
    sec_entries = struct.unpack(
        f"<{secondary_ts.metatiles_path.stat().st_size // 2}H", secondary_ts.metatiles_path.read_bytes())
    return (metatile_boundary, tile_local_offset, prim_img, prim_img.width // 8,
            sec_img, sec_img.width // 8, prim_entries, sec_entries)


def analyze_palette_usage(primary_ts, secondary_ts):
    """Read-only report: for every palette bank (primary owns [0, N), secondary
    owns [N, 13) where N = NUM_PALS_IN_PRIMARY), which color indices are
    actually used by tiles that are actually placed on a map, and a greedy
    proposal for how few banks that could be squeezed into. Writes nothing."""
    fmt, _ = detect_format(secondary_ts)
    num_pals_primary = NUM_PALS_IN_PRIMARY[fmt]
    metatile_boundary, tile_local_offset, prim_img, prim_cols, sec_img, sec_cols, prim_entries, sec_entries = \
        _load_tileset_pair(primary_ts, secondary_ts)

    # Scope each side independently by role, not by this one pairing: a primary
    # can be shared by multiple secondaries (as Basic_Interior is, by both
    # Startingtowninterior and Basement), so its bank usage must reflect every
    # layout that uses it as primary. Filter by numeric range so a placed ID
    # belonging to some OTHER secondary's own metatile space (e.g. Basement's)
    # never gets decoded through secondary_ts's metatiles.bin by mistake.
    used_ids = {mid for mid in _used_metatile_ids_for_role(primary_ts, "primary") if mid < metatile_boundary}
    used_ids |= {mid for mid in _used_metatile_ids_for_role(secondary_ts, "secondary") if mid >= metatile_boundary}

    bank_colors = _bank_colors_for_ids(fmt, primary_ts, secondary_ts, used_ids, metatile_boundary, tile_local_offset,
                                        prim_img, prim_cols, sec_img, sec_cols, prim_entries, sec_entries)

    print(f"\nPalette bank usage (of tiles actually placed on a map/border):")
    for bank in range(NUM_PALS_TOTAL):
        owner = "primary" if bank < num_pals_primary else "secondary"
        colors = bank_colors.get(bank, {})
        if not colors:
            print(f"  bank {bank:2} ({owner}): unused - free")
        else:
            print(f"  bank {bank:2} ({owner}): {len(colors)}/{PALETTE_BANK_CAPACITY} colors used")

    if not bank_colors:
        print("  No used banks found.")
        return

    groups = _bin_pack_banks(bank_colors)
    print(f"\n{len(bank_colors)} banks currently used -> {len(groups)} bank(s) if colors were freely "
          f"renumbered (theoretical - requires rewriting tile pixel data, not offered by this tool):")
    for i, group in enumerate(groups):
        src_banks = sorted({b for b, _ in group.values()})
        print(f"  theoretical bank {i}: {len(group)}/{PALETTE_BANK_CAPACITY} colors, "
              f"from original bank(s) {src_banks}")

    safe_groups = _bin_pack_banks_index_preserving(bank_colors)
    print(f"\n-> {len(safe_groups)} bank(s) achievable WITHOUT touching any pixel data "
          f"('Consolidate palettes' action - colors must stay at their original index, "
          f"so banks only merge when their used indexes don't collide):")
    for i, group in enumerate(safe_groups):
        src_banks = sorted({b for _, b in group.values()})
        print(f"  safe bank {num_pals_primary+i}: {len(group)}/{PALETTE_BANK_CAPACITY} colors at index(es) "
              f"{sorted(group.keys())}, from original bank(s) {src_banks}")
    if any(b < num_pals_primary for b in {b for group in safe_groups for _, b in group.values()}):
        print(f"  Note: {primary_ts.name} is shared by other secondaries too (checked - see "
              f"'primary' rows above), so its own banks are never renumbered by the apply step; "
              f"only {secondary_ts.name}'s own bank slots ({num_pals_primary}-{NUM_PALS_TOTAL-1}) get rewritten.")


def apply_palette_consolidation(primary_ts, secondary_ts, ask_restore=True):
    """Consolidate every palette bank that secondary_ts's OWN metatiles
    reference (including ones it borrows from primary_ts) into as few of
    secondary_ts's OWN bank slots as possible. Never touches primary_ts's
    files or any other tileset - a tile's pixel data is left exactly where it
    is (in whichever tiles.png), only the bank NUMBER a metatile entry selects
    changes, and only within secondary_ts's own metatiles.bin. This means it's
    safe even when the primary is shared with other secondaries (e.g. Basement),
    since their bank numbers/content are never touched.

    Bank-to-file mapping (confirmed from LoadTilesetPalette in src/fieldmap.c):
    a tileset's .pal array INDEX equals the GLOBAL bank number directly for
    BOTH primary and secondary tilesets - there is no "-6" local offset for
    secondary. Secondary owns banks [NUM_PALS_IN_PRIMARY, NUM_PALS_TOTAL)."""
    fmt, _ = detect_format(secondary_ts)
    num_pals_primary = NUM_PALS_IN_PRIMARY[fmt]
    secondary_bank_range = range(num_pals_primary, NUM_PALS_TOTAL)

    metatile_boundary, tile_local_offset, prim_img, prim_cols, sec_img, sec_cols, prim_entries, sec_entries = \
        _load_tileset_pair(primary_ts, secondary_ts)

    used_local = find_used_local_metatile_indices(secondary_ts)
    used_ids = {local_idx + metatile_boundary for local_idx in used_local}
    bank_colors = _bank_colors_for_ids(fmt, primary_ts, secondary_ts, used_ids, metatile_boundary, tile_local_offset,
                                        prim_img, prim_cols, sec_img, sec_cols, prim_entries, sec_entries)
    if not bank_colors:
        print(f"  No used palette banks found for {secondary_ts.name}. Nothing to do.")
        return False

    # Index-preserving packing: pixel data is never rewritten (primary's tiles
    # can't be touched - Basement also reads them), so a color must stay at
    # its original index within whatever bank ends up hosting it. Groups here
    # are {index: (rgb, source_bank)}.
    groups = _bin_pack_banks_index_preserving(bank_colors)
    if len(groups) > len(secondary_bank_range):
        print(f"  ! {secondary_ts.name}'s footprint needs {len(groups)} banks, but only "
              f"{len(secondary_bank_range)} secondary slots "
              f"({secondary_bank_range.start}-{secondary_bank_range.stop-1}) exist. "
              f"Aborting - can't fit without touching primary's banks.")
        return False

    old_bank_to_new = {}
    for i, group in enumerate(groups):
        new_bank = num_pals_primary + i
        for c, old_bank in group.values():
            old_bank_to_new[old_bank] = new_bank

    print(f"\n{len(bank_colors)} bank(s) used by {secondary_ts.name} -> consolidating into "
          f"{len(groups)} of its own bank slot(s) (index-preserving, no pixel data touched):")
    for i, group in enumerate(groups):
        src_banks = sorted({b for _, b in group.values()})
        print(f"  bank {num_pals_primary+i} <- {len(group)}/{PALETTE_BANK_CAPACITY} colors at "
              f"original index(es) {sorted(group.keys())}, from original bank(s) {src_banks}")

    # Rewrite secondary_ts's own metatiles.bin: bank-select bits only, tile_id
    # (which physical tiles.png/index a slot points at) is never touched.
    metatiles_backup = secondary_ts.metatiles_path.with_suffix(secondary_ts.metatiles_path.suffix + ".bak")
    if not metatiles_backup.exists():
        metatiles_backup.write_bytes(secondary_ts.metatiles_path.read_bytes())
        print(f"  Backed up {metatiles_backup.relative_to(ROOT)}")

    new_entries = list(sec_entries)
    changed = 0
    for i, v in enumerate(new_entries):
        old_bank = (v & 0xF000) >> 12
        new_bank = old_bank_to_new.get(old_bank)
        if new_bank is None or new_bank == old_bank:
            continue
        new_entries[i] = (v & ~0xF000) | (new_bank << 12)
        changed += 1
    secondary_ts.metatiles_path.write_bytes(struct.pack(f"<{len(new_entries)}H", *new_entries))
    print(f"  Rewrote bank-select bits on {changed} tile-slot(s) in "
          f"{secondary_ts.metatiles_path.relative_to(ROOT)}")

    # Rewrite secondary_ts's own .pal source files: array index num_pals_primary+i
    # (bank num_pals_primary+i) gets the consolidated group's colors; index 0
    # stays whatever it was (never referenced, purely a placeholder); any real
    # hardware slot not targeted by a group is no longer referenced by
    # anything and gets blanked to black.
    #
    # IMPORTANT: only touch array indices in secondary_bank_range (the real
    # hardware-addressable secondary banks). Indices BELOW num_pals_primary
    # (e.g. files 00-05.pal for emerald) are never read by the engine at all
    # for a secondary tileset - confirmed from fieldmap.c - so they must never
    # be treated as "ours." Same for indices beyond NUM_PALS_TOTAL if present.
    pal_backups = []
    targeted_banks = {num_pals_primary + i for i in range(len(groups))}
    for bank in secondary_bank_range:
        if bank >= len(secondary_ts.palette_paths):
            break
        pal_path = secondary_ts.palette_paths[bank].with_suffix(".pal")
        backup = pal_path.with_suffix(".pal.bak")
        if not backup.exists():
            backup.write_bytes(pal_path.read_bytes())
        pal_backups.append((pal_path, backup))

        if bank in targeted_banks:
            group = groups[bank - num_pals_primary]
            original = read_jasc_pal(pal_path)
            new_colors = list(original)  # keep index 0 as-is (transparent, content doesn't matter)
            for idx, (c, _src_bank) in group.items():
                new_colors[idx] = c  # preserved at its ORIGINAL index - pixel data still resolves correctly
            for idx in range(1, 16):
                if idx not in group:
                    new_colors[idx] = (0, 0, 0)  # not part of this group, never read - blank it
        else:
            new_colors = [(0, 0, 0)] * 16  # not referenced by anything anymore

        lines = ["JASC-PAL", "0100", "16"] + [f"{r} {g} {b}" for r, g, b in new_colors]
        pal_path.write_text("\n".join(lines) + "\n")

    print(f"  Wrote {len(pal_backups)} palette file(s) (banks "
          f"{secondary_bank_range.start}-{secondary_bank_range.start + len(pal_backups) - 1}) in "
          f"{secondary_ts.dir.relative_to(ROOT)}/palettes/ - files below bank "
          f"{num_pals_primary} left untouched (not read by the engine for a secondary tileset)")

    if _prompt_restore("metatiles.bin and palette files", ask_restore):
        secondary_ts.metatiles_path.write_bytes(metatiles_backup.read_bytes())
        for pal_path, backup in pal_backups:
            pal_path.write_bytes(backup.read_bytes())
        print("  Restored original metatiles.bin and palette files.")
    return True


def apply_secondary_palette_minimization(primary_ts, secondary_ts, ask_restore=True):
    """Repack secondary_ts's OWN palette bank slots into as few of them as
    possible, WITHOUT touching primary_ts and WITHOUT duplicating any color
    data that secondary_ts already gets for free by borrowing one of
    primary_ts's banks. Any bank a secondary metatile references that belongs
    to the primary (bank < NUM_PALS_IN_PRIMARY) is pinned exactly as-is - it
    is never renumbered, never copied into a secondary .pal file, and
    primary_ts's files are never opened for writing. This is the opposite
    trade-off from apply_palette_consolidation (which folds borrowed primary
    banks into the secondary's own slots too, e.g. in preparation for a merge
    that absorbs the primary) - here the goal is purely to shrink the
    secondary's own bank footprint while keeping the free ride from the
    primary intact."""
    fmt, _ = detect_format(secondary_ts)
    num_pals_primary = NUM_PALS_IN_PRIMARY[fmt]
    secondary_bank_range = range(num_pals_primary, NUM_PALS_TOTAL)

    metatile_boundary, tile_local_offset, prim_img, prim_cols, sec_img, sec_cols, prim_entries, sec_entries = \
        _load_tileset_pair(primary_ts, secondary_ts)

    used_local = find_used_local_metatile_indices(secondary_ts)
    used_ids = {local_idx + metatile_boundary for local_idx in used_local}
    bank_colors = _bank_colors_for_ids(fmt, primary_ts, secondary_ts, used_ids, metatile_boundary, tile_local_offset,
                                        prim_img, prim_cols, sec_img, sec_cols, prim_entries, sec_entries)
    if not bank_colors:
        print(f"  No used palette banks found for {secondary_ts.name}. Nothing to do.")
        return False

    borrowed_primary_banks = sorted(b for b in bank_colors if b < num_pals_primary)
    own_bank_colors = {b: c for b, c in bank_colors.items() if b in secondary_bank_range}
    if not own_bank_colors:
        print(f"  {secondary_ts.name} only uses primary bank(s) {borrowed_primary_banks} - "
              f"nothing of its own to consolidate.")
        return True

    # Index-preserving packing, scoped to the secondary's OWN banks only -
    # borrowed primary banks never enter the packer, so they can never end up
    # merged into (or evicting) one of the secondary's own slots.
    groups = _bin_pack_banks_index_preserving(own_bank_colors)
    if len(groups) > len(secondary_bank_range):
        print(f"  ! {secondary_ts.name}'s own bank footprint needs {len(groups)} banks, but only "
              f"{len(secondary_bank_range)} secondary slots "
              f"({secondary_bank_range.start}-{secondary_bank_range.stop-1}) exist. Aborting.")
        return False

    old_bank_to_new = {}
    for i, group in enumerate(groups):
        new_bank = num_pals_primary + i
        for c, old_bank in group.values():
            old_bank_to_new[old_bank] = new_bank

    if borrowed_primary_banks:
        print(f"\n  Pinned - borrowed from {primary_ts.name}, left untouched: "
              f"bank(s) {borrowed_primary_banks}")
    print(f"  {len(own_bank_colors)} of {secondary_ts.name}'s own bank(s) -> consolidating into "
          f"{len(groups)} of its own bank slot(s) (index-preserving, no pixel data touched):")
    for i, group in enumerate(groups):
        src_banks = sorted({b for _, b in group.values()})
        print(f"  bank {num_pals_primary+i} <- {len(group)}/{PALETTE_BANK_CAPACITY} colors at "
              f"original index(es) {sorted(group.keys())}, from original bank(s) {src_banks}")

    metatiles_backup = secondary_ts.metatiles_path.with_suffix(secondary_ts.metatiles_path.suffix + ".bak")
    if not metatiles_backup.exists():
        metatiles_backup.write_bytes(secondary_ts.metatiles_path.read_bytes())
        print(f"  Backed up {metatiles_backup.relative_to(ROOT)}")

    # old_bank_to_new only ever contains keys from own_bank_colors (secondary's
    # own bank range), so a tile-slot whose bank belongs to the primary simply
    # has no entry here and is left completely alone.
    new_entries = list(sec_entries)
    changed = 0
    for i, v in enumerate(new_entries):
        old_bank = (v & 0xF000) >> 12
        new_bank = old_bank_to_new.get(old_bank)
        if new_bank is None or new_bank == old_bank:
            continue
        new_entries[i] = (v & ~0xF000) | (new_bank << 12)
        changed += 1
    secondary_ts.metatiles_path.write_bytes(struct.pack(f"<{len(new_entries)}H", *new_entries))
    print(f"  Rewrote bank-select bits on {changed} tile-slot(s) in "
          f"{secondary_ts.metatiles_path.relative_to(ROOT)}")

    pal_backups = []
    targeted_banks = {num_pals_primary + i for i in range(len(groups))}
    for bank in secondary_bank_range:
        if bank >= len(secondary_ts.palette_paths):
            break
        pal_path = secondary_ts.palette_paths[bank].with_suffix(".pal")
        backup = pal_path.with_suffix(".pal.bak")
        if not backup.exists():
            backup.write_bytes(pal_path.read_bytes())
        pal_backups.append((pal_path, backup))

        if bank in targeted_banks:
            group = groups[bank - num_pals_primary]
            original = read_jasc_pal(pal_path)
            new_colors = list(original)  # keep index 0 as-is (transparent, content doesn't matter)
            for idx, (c, _src_bank) in group.items():
                new_colors[idx] = c  # preserved at its ORIGINAL index - pixel data still resolves correctly
            for idx in range(1, 16):
                if idx not in group:
                    new_colors[idx] = (0, 0, 0)  # not part of this group, never read - blank it
        else:
            new_colors = [(0, 0, 0)] * 16  # not referenced by anything anymore

        lines = ["JASC-PAL", "0100", "16"] + [f"{r} {g} {b}" for r, g, b in new_colors]
        pal_path.write_text("\n".join(lines) + "\n")

    print(f"  Wrote {len(pal_backups)} palette file(s) (banks "
          f"{secondary_bank_range.start}-{secondary_bank_range.start + len(pal_backups) - 1}) in "
          f"{secondary_ts.dir.relative_to(ROOT)}/palettes/ - {primary_ts.name}'s files untouched")

    if _prompt_restore("metatiles.bin and palette files", ask_restore):
        secondary_ts.metatiles_path.write_bytes(metatiles_backup.read_bytes())
        for pal_path, backup in pal_backups:
            pal_path.write_bytes(backup.read_bytes())
        print("  Restored original metatiles.bin and palette files.")
    return True


def find_partner_tilesets(ts, tilesets):
    """Every tileset ts is ever paired with (as primary<->secondary) across
    layouts.json - these can reference into ts's own tile range, the same way
    interior_house borrows basic_interior's tiles."""
    layouts = json.loads(LAYOUTS_JSON.read_text())["layouts"]
    partners = set()
    for layout in layouts:
        prim, sec = layout.get("primary_tileset"), layout.get("secondary_tileset")
        if prim == ts.name and sec in tilesets:
            partners.add(sec)
        elif sec == ts.name and prim in tilesets:
            partners.add(prim)
    return [tilesets[name] for name in partners]


def prune_unused(ts, tilesets, ask_restore=True):
    """Two-step cleanup for metatiles/tiles that no map or border actually
    places: first zero out unused metatile entries (tile-slots -> tile 0,
    attributes -> 0), then blank any physical tile no longer referenced by any
    (now-updated) metatile. Metatile IDs/order never change, so no map.bin
    remapping is needed - this only zeroes data nothing points at."""
    fmt, entry_size = detect_format(ts)
    if entry_size is None:
        print(f"  ! Could not determine format for {ts.name}. Aborting.")
        return False

    used_metatiles = find_used_local_metatile_indices(ts)
    metatiles_data = bytearray(ts.metatiles_path.read_bytes())
    attrs_data = bytearray(ts.attrs_path.read_bytes())
    num_metatiles = len(metatiles_data) // METATILE_BYTES
    unused_metatiles = [i for i in range(num_metatiles) if i not in used_metatiles]

    metatiles_backup = ts.metatiles_path.with_suffix(ts.metatiles_path.suffix + ".bak")
    attrs_backup = ts.attrs_path.with_suffix(ts.attrs_path.suffix + ".bak")
    tiles_png_path = ts.dir / "tiles.png"
    png_backup = tiles_png_path.with_suffix(tiles_png_path.suffix + ".bak")

    if not unused_metatiles:
        print(f"  No unused metatiles ({num_metatiles} defined, all referenced by some map/border).")
    else:
        if not metatiles_backup.exists():
            metatiles_backup.write_bytes(bytes(metatiles_data))
            print(f"  Backed up {metatiles_backup.relative_to(ROOT)}")
        if not attrs_backup.exists():
            attrs_backup.write_bytes(bytes(attrs_data))
            print(f"  Backed up {attrs_backup.relative_to(ROOT)}")
        for i in unused_metatiles:
            metatiles_data[i * METATILE_BYTES:(i + 1) * METATILE_BYTES] = b"\x00" * METATILE_BYTES
            attrs_data[i * entry_size:(i + 1) * entry_size] = b"\x00" * entry_size
        ts.metatiles_path.write_bytes(bytes(metatiles_data))
        ts.attrs_path.write_bytes(bytes(attrs_data))
        print(f"  Wiped {len(unused_metatiles)}/{num_metatiles} unused metatile(s) "
              f"(not placed on any map or border)")

    # Step 2: with unused metatiles now zeroed, find tiles no longer referenced
    # anywhere in this tileset's own local tile range and blank them. This must
    # also check every paired tileset's metatiles.bin, since a secondary can
    # borrow tiles directly from its primary's range (and vice versa) - only
    # checking ts's own metatiles.bin would blank tiles still in active use,
    # the same bug that broke basic_interior/interior_house earlier.
    local_offset = NUM_TILES_IN_PRIMARY[fmt] if ts.is_secondary else 0
    local_hi = NUM_TILES_TOTAL if ts.is_secondary else NUM_TILES_IN_PRIMARY[fmt]

    def tile_refs(data):
        n = len(data) // 2
        for v in struct.unpack(f"<{n}H", data):
            tid = v & TILE_ID_MASK
            if local_offset <= tid < local_hi:
                yield tid - local_offset

    used_tiles = set(tile_refs(bytes(metatiles_data)))
    partners = find_partner_tilesets(ts, tilesets)
    for partner in partners:
        used_tiles.update(tile_refs(partner.metatiles_path.read_bytes()))
    if partners:
        print(f"  (also checked {len(partners)} paired tileset(s) for cross-references: "
              f"{', '.join(p.name for p in partners)})")

    img = png_util.read_indexed_png(tiles_png_path)
    num_cols = img.width // 8
    num_tiles = (img.height // 8) * num_cols
    unused_tiles = [i for i in range(num_tiles) if i not in used_tiles]

    if not unused_tiles:
        print(f"  No unused tiles ({num_tiles} in sheet, all referenced by some metatile).")
    else:
        if not png_backup.exists():
            png_backup.write_bytes(tiles_png_path.read_bytes())
            print(f"  Backed up {png_backup.relative_to(ROOT)}")
        for i in unused_tiles:
            col, row = i % num_cols, i // num_cols
            ox, oy = col * 8, row * 8
            for dy in range(8):
                base = (oy + dy) * img.width + ox
                img.pixels[base:base + 8] = bytes(8)
        png_util.write_indexed_png(tiles_png_path, img)
        print(f"  Blanked {len(unused_tiles)}/{num_tiles} unused tile(s) in tiles.png")

    if not unused_metatiles and not unused_tiles:
        return True

    if _prompt_restore("metatiles.bin/metatile_attributes.bin/tiles.png", ask_restore):
        if metatiles_backup.exists():
            ts.metatiles_path.write_bytes(metatiles_backup.read_bytes())
        if attrs_backup.exists():
            ts.attrs_path.write_bytes(attrs_backup.read_bytes())
        if png_backup.exists():
            tiles_png_path.write_bytes(png_backup.read_bytes())
        print("  Restored original metatiles.bin, metatile_attributes.bin and tiles.png.")
    return True


def find_paired_primaries(secondary_ts):
    """Every distinct primary_tileset that pairs with this secondary across layouts.json."""
    layouts = json.loads(LAYOUTS_JSON.read_text())["layouts"]
    prims = set()
    for layout in layouts:
        if layout.get("secondary_tileset") == secondary_ts.name:
            prims.add(layout.get("primary_tileset"))
    return prims


def _new_c_symbol(new_dir_name):
    return "gTileset_" + "".join(part.capitalize() for part in new_dir_name.split("_"))


def merge_secondary_absorbing_primary(primary_ts, secondary_ts, new_dir_name):
    """Build a new, self-contained secondary tileset that owns copies of every
    primary tile the given secondary currently borrows by cross-referencing into
    the primary's tiles.png. Metatile IDs/order are unchanged (map.bin/blockdata
    is untouched) - only the internal tile-slot references inside each metatile
    are rewritten to point into the new tileset's own tile sheet."""
    fmt, _ = detect_format(secondary_ts)
    local_offset = NUM_TILES_IN_PRIMARY[fmt]

    sec_img = png_util.read_indexed_png(secondary_ts.dir / "tiles.png")
    sec_cols = sec_img.width // 8
    sec_num_tiles = (sec_img.height // 8) * sec_cols
    self_tiles = [_tile_pixels(sec_img, i, sec_cols) for i in range(sec_num_tiles)]

    prim_img = png_util.read_indexed_png(primary_ts.dir / "tiles.png")
    prim_cols = prim_img.width // 8

    data = secondary_ts.metatiles_path.read_bytes()
    n = len(data) // 2
    entries = list(struct.unpack(f"<{n}H", data))

    referenced_primary_ids = sorted({v & TILE_ID_MASK for v in entries if (v & TILE_ID_MASK) < local_offset})
    primary_id_to_combined = {}
    primary_tiles_copied = []
    for pid in referenced_primary_ids:
        primary_id_to_combined[pid] = len(self_tiles) + len(primary_tiles_copied)
        primary_tiles_copied.append(_tile_pixels(prim_img, pid, prim_cols))

    combined_tiles = self_tiles + primary_tiles_copied
    unique, mapping = _dedupe_tiles(combined_tiles)
    print(f"  {len(self_tiles)} own tiles + {len(primary_tiles_copied)} borrowed primary tiles "
          f"-> {len(unique)} unique after merge+dedupe")

    num_cols = sec_cols
    new_rows = (len(unique) + num_cols - 1) // num_cols
    new_height = new_rows * 8
    new_pixels = bytearray(sec_img.width * new_height)
    for idx, tile in enumerate(unique):
        col, row = idx % num_cols, idx // num_cols
        ox, oy = col * 8, row * 8
        for dy in range(8):
            new_pixels[(oy + dy) * sec_img.width + ox: (oy + dy) * sec_img.width + ox + 8] = \
                tile[dy * 8: dy * 8 + 8]
    new_img = png_util.IndexedPng(sec_img.width, new_height, sec_img.bit_depth, new_pixels, sec_img.chunks)

    new_entries = []
    for v in entries:
        tid = v & TILE_ID_MASK
        if tid < local_offset:
            combined_idx = primary_id_to_combined.get(tid)
        elif local_offset <= tid < local_offset + len(self_tiles):
            combined_idx = tid - local_offset
        else:
            combined_idx = None
        if combined_idx is None:
            new_entries.append(v)
            continue
        new_local, fx, fy = mapping[combined_idx]
        hflip = bool(v & TILE_HFLIP_BIT) ^ fx
        vflip = bool(v & TILE_VFLIP_BIT) ^ fy
        pal_bits = v & ~(TILE_ID_MASK | TILE_HFLIP_BIT | TILE_VFLIP_BIT)
        nv = pal_bits | ((new_local + local_offset) & TILE_ID_MASK)
        if hflip:
            nv |= TILE_HFLIP_BIT
        if vflip:
            nv |= TILE_VFLIP_BIT
        new_entries.append(nv)

    new_dir = secondary_ts.dir.parent / new_dir_name
    new_dir.mkdir(exist_ok=True)
    png_util.write_indexed_png(new_dir / "tiles.png", new_img)
    (new_dir / "metatiles.bin").write_bytes(struct.pack(f"<{n}H", *new_entries))
    (new_dir / "metatile_attributes.bin").write_bytes(secondary_ts.attrs_path.read_bytes())

    # Only copy as many palettes as the original secondary actually wires up in
    # graphics.h (its palettes/ dir may contain extra unused .pal files beyond that).
    pal_dir = new_dir / "palettes"
    pal_dir.mkdir(exist_ok=True)
    pal_count = len(secondary_ts.palette_paths)
    for i in range(pal_count):
        src_pal = (secondary_ts.dir / "palettes" / f"{i:02d}.gbapal").with_suffix(".pal")
        (pal_dir / src_pal.name).write_bytes(src_pal.read_bytes())

    print(f"  Wrote new tileset dir {new_dir.relative_to(ROOT)}/ "
          f"({new_rows} rows, {len(unique)} tiles, {pal_count} palettes)")
    return new_dir, pal_count


def register_tileset_c_source(new_symbol, new_dir_name, is_compressed, pal_count):
    """Append struct/INCBIN declarations for a brand-new secondary tileset to
    headers.h / metatiles.h / graphics.h. Appending is safe: these are all
    top-level const declarations with no ordering requirements."""
    rel_dir = f"data/tilesets/secondary/{new_dir_name}"
    tiles_sym = f"gTilesetTiles_{new_symbol[len('gTileset_'):]}"
    pal_sym = f"gTilesetPalettes_{new_symbol[len('gTileset_'):]}"
    metatiles_sym = f"gMetatiles_{new_symbol[len('gTileset_'):]}"
    attrs_sym = f"gMetatileAttributes_{new_symbol[len('gTileset_'):]}"

    metatiles_block = (
        f'\nconst u16 {metatiles_sym}[] = INCBIN_U16("{rel_dir}/metatiles.bin");\n'
        f'const u16 {attrs_sym}[] = INCBIN_U16("{rel_dir}/metatile_attributes.bin");\n'
    )
    with open(METATILES_H, "a") as f:
        f.write(metatiles_block)

    pal_lines = "\n".join(
        f'    INCBIN_U16("{rel_dir}/palettes/{i:02d}.gbapal"),' for i in range(pal_count)
    )
    tiles_ext = "tiles.4bpp.lz" if is_compressed else "tiles.4bpp"
    graphics_block = (
        f"\nconst u16 {pal_sym}[][16] =\n{{\n{pal_lines}\n}};\n\n"
        f'const u32 {tiles_sym}[] = INCBIN_U32("{rel_dir}/{tiles_ext}");\n'
    )
    with open(GRAPHICS_H, "a") as f:
        f.write(graphics_block)

    headers_block = (
        f"\nconst struct Tileset {new_symbol} =\n{{\n"
        f'    .isCompressed = {"TRUE" if is_compressed else "FALSE"},\n'
        f"    .isSecondary = TRUE,\n"
        f"    .tiles = {tiles_sym},\n"
        f"    .palettes = {pal_sym},\n"
        f"    .metatiles = {metatiles_sym},\n"
        f"    .metatileAttributes = {attrs_sym},\n"
        f"    .callback = NULL,\n"
        f"}};\n"
    )
    with open(HEADERS_H, "a") as f:
        f.write(headers_block)

    print(f"  Registered {new_symbol} in headers.h, metatiles.h, graphics.h")


def repoint_layouts(old_secondary_name, primary_name, new_symbol):
    """Rewrite secondary_tileset for layouts pairing old_secondary_name with
    primary_name, in place, touching only the matching lines (keeps the rest of
    layouts.json byte-for-byte unchanged)."""
    text = LAYOUTS_JSON.read_text()
    layout_block_re = re.compile(
        r'(\{\s*"id":\s*"[^"]+".*?\})', re.DOTALL
    )
    changed = 0

    def repl(m):
        nonlocal changed
        block = m.group(1)
        if (f'"secondary_tileset": "{old_secondary_name}"' in block
                and f'"primary_tileset": "{primary_name}"' in block):
            changed += 1
            return block.replace(
                f'"secondary_tileset": "{old_secondary_name}"',
                f'"secondary_tileset": "{new_symbol}"',
            )
        return block

    new_text = layout_block_re.sub(repl, text)
    LAYOUTS_JSON.write_text(new_text)
    print(f"  Updated secondary_tileset in {changed} layout(s) in "
          f"{LAYOUTS_JSON.relative_to(ROOT)}")
    return changed


def merge_tileset_pair(tilesets, secondary_ts, ask_restore=True):
    paired_primaries = find_paired_primaries(secondary_ts)
    if len(paired_primaries) != 1:
        print(f"  ! {secondary_ts.name} is paired with {len(paired_primaries)} different "
              f"primaries ({sorted(p for p in paired_primaries if p)}); this tool only "
              f"supports merging an unambiguous 1:1 pairing. Aborting.")
        return
    primary_name = next(iter(paired_primaries))
    primary_ts = tilesets.get(primary_name)
    if not primary_ts:
        print(f"  ! Could not resolve primary tileset {primary_name}. Aborting.")
        return

    default_dir = secondary_ts.dir.name + "_merged"
    new_dir_name = input(f"New tileset directory name [{default_dir}]: ").strip() or default_dir
    new_symbol = _new_c_symbol(new_dir_name)
    if new_symbol in tilesets:
        print(f"  ! {new_symbol} already exists. Aborting.")
        return

    # Snapshot every file this operation will edit in place, so it can be
    # cleanly undone regardless of what register_tileset_c_source/repoint_layouts do.
    snapshots = {p: p.read_text() for p in (HEADERS_H, METATILES_H, GRAPHICS_H, LAYOUTS_JSON)}

    print(f"\nMerging {secondary_ts.name} (secondary) + tiles borrowed from "
          f"{primary_ts.name} (primary) -> new tileset {new_symbol}")
    new_dir, pal_count = merge_secondary_absorbing_primary(primary_ts, secondary_ts, new_dir_name)
    register_tileset_c_source(new_symbol, new_dir_name, secondary_ts.is_compressed, pal_count)
    repoint_layouts(secondary_ts.name, primary_ts.name, new_symbol)
    print(f"\n{secondary_ts.name} and its files were left untouched - "
          f"delete them once you've verified {new_symbol} in-game.")

    if _prompt_restore(f"(delete {new_dir_name}/ and undo the headers.h/metatiles.h/"
                        f"graphics.h/layouts.json edits)", ask_restore):
        for path, text in snapshots.items():
            path.write_text(text)
        import shutil
        shutil.rmtree(new_dir, ignore_errors=True)
        print(f"  Restored. Deleted {new_dir_name}/ and reverted headers.h/metatiles.h/"
              f"graphics.h/layouts.json.")
    else:
        print("  Kept.")


def print_tileset_list(tilesets):
    names = sorted(tilesets.keys())
    for i, name in enumerate(names, 1):
        ts = tilesets[name]
        role = "secondary" if ts.is_secondary else "primary"
        fmt, _ = detect_format(ts)
        print(f"{i:3}. {name:<40} [{role:<9}] {fmt or '?':<8} "
              f"used by {len(ts.maps)} map(s)")
    return names


def show_tileset_detail(ts):
    fmt, entry_size = detect_format(ts)
    role = "secondary" if ts.is_secondary else "primary"
    print(f"\n{ts.name}  ({role}, {fmt} format)")
    print(f"  dir: {ts.dir.relative_to(ROOT) if ts.dir else '?'}")
    if ts.maps:
        print(f"  used by {len(ts.maps)} map(s):")
        for map_name, r in sorted(set(ts.maps)):
            print(f"    - {map_name} ({r})")
    else:
        print("  used by 0 maps (not referenced by any layout)")


def interactive(tilesets, args):
    names = print_tileset_list(tilesets)
    choice = input("\nSelect a tileset by number (or blank to quit): ").strip()
    if not choice:
        return
    try:
        idx = int(choice)
        name = names[idx - 1]
    except (ValueError, IndexError):
        print("Invalid selection.")
        return
    ts = tilesets[name]
    show_tileset_detail(ts)

    print("\nActions:")
    print("  1. Wipe metatile data (behavior -> MB_NORMAL, terrain -> NORMAL, "
          "encounter -> NONE, layer type -> NORMAL)")
    print("  2. Deduplicate tiles (merge identical/mirrored tiles in tiles.png, "
          "update metatiles.bin)")
    if ts.is_secondary:
        print("  3. Make standalone (absorb tiles borrowed from its paired primary "
              "into a new, self-contained secondary tileset)")
    print("  4. Prune unused (wipe metatiles not placed on any map/border, "
          "then blank tiles no metatile references)")
    if ts.is_secondary:
        print("  5. Analyze palette usage (report only, no files changed)")
        print("  6. Consolidate palettes (repack this tileset's own bank usage "
              "into fewer of its own bank slots)")
        print("  7. Minimize own palette usage (repack only this tileset's own "
              "banks; banks borrowed from its primary are pinned, never touched "
              "or duplicated)")
    action = input("Select an action (or blank to quit): ").strip()
    if action == "1":
        confirm = input(
            f"This will overwrite {ts.attrs_path.relative_to(ROOT)} "
            f"(affects {len(ts.maps)} map(s)). Type 'yes' to continue: "
        ).strip().lower()
        if confirm == "yes":
            wipe_attributes(ts)
        else:
            print("Cancelled.")
    elif action == "2":
        confirm = input(
            f"This will overwrite {ts.dir.relative_to(ROOT)}/tiles.png and "
            f"{ts.metatiles_path.relative_to(ROOT)} (affects {len(ts.maps)} map(s)). "
            f"Type 'yes' to continue: "
        ).strip().lower()
        if confirm == "yes":
            dedupe_tileset(ts)
        else:
            print("Cancelled.")
    elif action == "3" and ts.is_secondary:
        confirm = input(
            f"This will create a new tileset directory, register it in headers.h/"
            f"metatiles.h/graphics.h, and update layouts.json (leaving {ts.name} "
            f"untouched). Type 'yes' to continue: "
        ).strip().lower()
        if confirm == "yes":
            merge_tileset_pair(tilesets, ts)
        else:
            print("Cancelled.")
    elif action == "4":
        confirm = input(
            f"This will overwrite {ts.metatiles_path.relative_to(ROOT)}, "
            f"{ts.attrs_path.relative_to(ROOT)} and {ts.dir.relative_to(ROOT)}/tiles.png "
            f"(affects {len(ts.maps)} map(s)). Type 'yes' to continue: "
        ).strip().lower()
        if confirm == "yes":
            prune_unused(ts, tilesets)
        else:
            print("Cancelled.")
    elif action == "5" and ts.is_secondary:
        paired = find_paired_primaries(ts)
        if len(paired) != 1:
            print(f"  ! {ts.name} is paired with {len(paired)} different primaries; "
                  f"this analysis only supports an unambiguous 1:1 pairing.")
        else:
            primary_ts = tilesets.get(next(iter(paired)))
            if primary_ts:
                analyze_palette_usage(primary_ts, ts)
    elif action == "6" and ts.is_secondary:
        paired = find_paired_primaries(ts)
        if len(paired) != 1:
            print(f"  ! {ts.name} is paired with {len(paired)} different primaries; "
                  f"this only supports an unambiguous 1:1 pairing.")
        else:
            primary_ts = tilesets.get(next(iter(paired)))
            if primary_ts:
                confirm = input(
                    f"This will overwrite {ts.metatiles_path.relative_to(ROOT)} and "
                    f"{ts.dir.relative_to(ROOT)}/palettes/*.pal (affects {len(ts.maps)} map(s)). "
                    f"{primary_ts.name}'s own files are never touched. Type 'yes' to continue: "
                ).strip().lower()
                if confirm == "yes":
                    apply_palette_consolidation(primary_ts, ts)
                else:
                    print("Cancelled.")
    elif action == "7" and ts.is_secondary:
        paired = find_paired_primaries(ts)
        if len(paired) != 1:
            print(f"  ! {ts.name} is paired with {len(paired)} different primaries; "
                  f"this only supports an unambiguous 1:1 pairing.")
        else:
            primary_ts = tilesets.get(next(iter(paired)))
            if primary_ts:
                confirm = input(
                    f"This will overwrite {ts.metatiles_path.relative_to(ROOT)} and "
                    f"{ts.dir.relative_to(ROOT)}/palettes/*.pal (affects {len(ts.maps)} map(s)). "
                    f"{primary_ts.name}'s own files are never touched. Type 'yes' to continue: "
                ).strip().lower()
                if confirm == "yes":
                    apply_secondary_palette_minimization(primary_ts, ts)
                else:
                    print("Cancelled.")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--tileset", help="Tileset symbol name, e.g. gTileset_Pokemon_Center")
    parser.add_argument("--wipe", action="store_true", help="Wipe metatile data for --tileset")
    parser.add_argument("--dedupe-tiles", action="store_true",
                         help="Merge identical/mirrored tiles for --tileset")
    parser.add_argument("--make-standalone", action="store_true",
                         help="Absorb tiles borrowed from --tileset's paired primary into a new secondary tileset")
    parser.add_argument("--prune-unused", action="store_true",
                         help="Wipe metatiles/tiles for --tileset that no map or border places")
    parser.add_argument("--analyze-palettes", action="store_true",
                         help="Report-only: palette bank usage for --tileset and its paired primary")
    parser.add_argument("--consolidate-palettes", action="store_true",
                         help="Repack --tileset's own palette bank usage into fewer of its own bank slots")
    parser.add_argument("--minimize-secondary-palettes", action="store_true",
                         help="Repack --tileset's own palette banks into fewer of its own slots, "
                              "pinning (never touching or duplicating) any bank borrowed from its primary")
    parser.add_argument("--yes", action="store_true",
                         help="Skip confirmation and restore prompts (non-interactive, keeps all changes)")
    args = parser.parse_args()

    incbin_paths = parse_incbin_paths()
    tiles_paths, palette_paths = parse_graphics_paths()
    tilesets = parse_tilesets(incbin_paths, tiles_paths, palette_paths)
    attach_map_usage(tilesets)

    if args.tileset:
        ts = tilesets.get(args.tileset)
        if not ts:
            print(f"Unknown tileset: {args.tileset}")
            sys.exit(1)
        show_tileset_detail(ts)
        if args.wipe:
            if not args.yes:
                confirm = input(
                    f"This will overwrite {ts.attrs_path.relative_to(ROOT)} "
                    f"(affects {len(ts.maps)} map(s)). Type 'yes' to continue: "
                ).strip().lower()
                if confirm != "yes":
                    print("Cancelled.")
                    return
            wipe_attributes(ts, ask_restore=not args.yes)
        if args.dedupe_tiles:
            if not args.yes:
                confirm = input(
                    f"This will overwrite {ts.dir.relative_to(ROOT)}/tiles.png and "
                    f"{ts.metatiles_path.relative_to(ROOT)} (affects {len(ts.maps)} map(s)). "
                    f"Type 'yes' to continue: "
                ).strip().lower()
                if confirm != "yes":
                    print("Cancelled.")
                    return
            dedupe_tileset(ts, ask_restore=not args.yes)
        if args.make_standalone:
            if not ts.is_secondary:
                print(f"  ! {ts.name} is a primary tileset; only secondary tilesets can be made standalone.")
                return
            if not args.yes:
                confirm = input(
                    f"This will create a new tileset directory, register it in headers.h/"
                    f"metatiles.h/graphics.h, and update layouts.json (leaving {ts.name} "
                    f"untouched). Type 'yes' to continue: "
                ).strip().lower()
                if confirm != "yes":
                    print("Cancelled.")
                    return
            merge_tileset_pair(tilesets, ts, ask_restore=not args.yes)
        if args.prune_unused:
            if not args.yes:
                confirm = input(
                    f"This will overwrite {ts.metatiles_path.relative_to(ROOT)}, "
                    f"{ts.attrs_path.relative_to(ROOT)} and {ts.dir.relative_to(ROOT)}/tiles.png "
                    f"(affects {len(ts.maps)} map(s)). Type 'yes' to continue: "
                ).strip().lower()
                if confirm != "yes":
                    print("Cancelled.")
                    return
            prune_unused(ts, tilesets, ask_restore=not args.yes)
        if args.analyze_palettes:
            if not ts.is_secondary:
                print(f"  ! {ts.name} is a primary tileset; pass its secondary instead.")
                return
            paired = find_paired_primaries(ts)
            if len(paired) != 1:
                print(f"  ! {ts.name} is paired with {len(paired)} different primaries; "
                      f"this analysis only supports an unambiguous 1:1 pairing.")
                return
            primary_ts = tilesets.get(next(iter(paired)))
            if primary_ts:
                analyze_palette_usage(primary_ts, ts)
        if args.consolidate_palettes:
            if not ts.is_secondary:
                print(f"  ! {ts.name} is a primary tileset; pass its secondary instead.")
                return
            paired = find_paired_primaries(ts)
            if len(paired) != 1:
                print(f"  ! {ts.name} is paired with {len(paired)} different primaries; "
                      f"this only supports an unambiguous 1:1 pairing.")
                return
            primary_ts = tilesets.get(next(iter(paired)))
            if not primary_ts:
                return
            if not args.yes:
                confirm = input(
                    f"This will overwrite {ts.metatiles_path.relative_to(ROOT)} and "
                    f"{ts.dir.relative_to(ROOT)}/palettes/*.pal (affects {len(ts.maps)} map(s)). "
                    f"{primary_ts.name}'s own files are never touched. Type 'yes' to continue: "
                ).strip().lower()
                if confirm != "yes":
                    print("Cancelled.")
                    return
            apply_palette_consolidation(primary_ts, ts, ask_restore=not args.yes)
        if args.minimize_secondary_palettes:
            if not ts.is_secondary:
                print(f"  ! {ts.name} is a primary tileset; pass its secondary instead.")
                return
            paired = find_paired_primaries(ts)
            if len(paired) != 1:
                print(f"  ! {ts.name} is paired with {len(paired)} different primaries; "
                      f"this only supports an unambiguous 1:1 pairing.")
                return
            primary_ts = tilesets.get(next(iter(paired)))
            if not primary_ts:
                return
            if not args.yes:
                confirm = input(
                    f"This will overwrite {ts.metatiles_path.relative_to(ROOT)} and "
                    f"{ts.dir.relative_to(ROOT)}/palettes/*.pal (affects {len(ts.maps)} map(s)). "
                    f"{primary_ts.name}'s own files are never touched. Type 'yes' to continue: "
                ).strip().lower()
                if confirm != "yes":
                    print("Cancelled.")
                    return
            apply_secondary_palette_minimization(primary_ts, ts, ask_restore=not args.yes)
        return

    interactive(tilesets, args)


if __name__ == "__main__":
    main()
