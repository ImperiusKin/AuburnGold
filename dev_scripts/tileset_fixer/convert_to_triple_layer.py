#!/usr/bin/env python3
"""
One-time data migration: convert every tileset's metatiles.bin from the
vanilla 8-tile-slot/16-byte-per-metatile layout to the triple-layer
12-tile-slot/24-byte-per-metatile layout (see wiki: Triple-layer-metatiles).

    python3 dev_scripts/tileset_fixer/convert_to_triple_layer.py [--yes]

For each metatile, the old 8 tile-slots are placed into the 12 new slots
according to its (existing, untouched) layer type attribute:
  NORMAL  (uses middle+top): bottom = empty,      middle = old[0:4], top = old[4:8]
  COVERED (uses bottom+mid): bottom = old[0:4],    middle = old[4:8], top = empty
  SPLIT   (uses bottom+top): bottom = old[0:4],    middle = empty,    top = old[4:8]

metatile_attributes.bin is left completely untouched - its byte layout does
not change, and this fork's decoration.c still reads the layer-type field
at runtime for secret-base decoration passability, so it must survive the
migration with its original value (unlike the reference conversion script,
which zeroes it out - safe for vanilla pokeemerald, not safe here).

Handles both attribute formats already used in this repo: 2-byte "emerald"
entries (layer type at bits 12-15) and 4-byte "frlg" entries (bits 29-30) -
see METATILE_ATTR_LAYER_MASK(_FRLG) in include/global.fieldmap.h.

Writes metatiles.bin.bak next to each file the first time (skipped if a
.bak already exists), so a tileset can be reverted with:
    mv metatiles.bin.bak metatiles.bin
"""
import argparse
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
TILESETS_DIR = ROOT / "data/tilesets"

OLD_TILES_PER_METATILE = 8
NEW_TILES_PER_METATILE = 12
OLD_METATILE_BYTES = OLD_TILES_PER_METATILE * 2
NEW_METATILE_BYTES = NEW_TILES_PER_METATILE * 2

LAYER_NORMAL, LAYER_COVERED, LAYER_SPLIT = 0, 1, 2

EMERALD_LAYER_MASK, EMERALD_LAYER_SHIFT = 0xF000, 12
FRLG_LAYER_MASK, FRLG_LAYER_SHIFT = 0x60000000, 29


def detect_old_format(metatiles_size, attrs_size):
    """Returns (fmt, attr_entry_bytes, num_metatiles) or (None, None, None)
    if this tileset doesn't look like it's still in the pre-conversion
    8-tile-slot layout (already converted, or sizes don't line up)."""
    if metatiles_size % OLD_METATILE_BYTES != 0:
        return None, None, None
    num_metatiles = metatiles_size // OLD_METATILE_BYTES
    if num_metatiles == 0:
        return None, None, None
    if attrs_size == num_metatiles * 2:
        return "emerald", 2, num_metatiles
    if attrs_size == num_metatiles * 4:
        return "frlg", 4, num_metatiles
    return None, None, None


def layer_type_for(fmt, raw_attr):
    if fmt == "emerald":
        return (raw_attr & EMERALD_LAYER_MASK) >> EMERALD_LAYER_SHIFT
    return (raw_attr & FRLG_LAYER_MASK) >> FRLG_LAYER_SHIFT


def convert_metatile(old_tiles, layer_type):
    zero4 = (0, 0, 0, 0)
    bottom, middle, top = old_tiles[0:4], old_tiles[4:8], None
    if layer_type == LAYER_NORMAL:
        return zero4 + old_tiles[0:4] + old_tiles[4:8]
    elif layer_type == LAYER_COVERED:
        return old_tiles[0:4] + old_tiles[4:8] + zero4
    elif layer_type == LAYER_SPLIT:
        return old_tiles[0:4] + zero4 + old_tiles[4:8]
    else:
        return zero4 + zero4 + zero4  # unrecognized layer type - blank rather than guess


def convert_tileset_dir(ts_dir: Path, apply: bool):
    metatiles_path = ts_dir / "metatiles.bin"
    attrs_path = ts_dir / "metatile_attributes.bin"
    name = ts_dir.name

    if not metatiles_path.is_file() or not attrs_path.is_file():
        print(f"[SKIP] {name}: missing metatiles.bin or metatile_attributes.bin")
        return "skip"

    metatiles_size = metatiles_path.stat().st_size
    attrs_size = attrs_path.stat().st_size
    fmt, attr_bytes, num_metatiles = detect_old_format(metatiles_size, attrs_size)
    if fmt is None:
        if metatiles_size % NEW_METATILE_BYTES == 0 and metatiles_size // NEW_METATILE_BYTES * attr_bytes_guess(attrs_size, metatiles_size) == attrs_size:
            print(f"[SKIP] {name}: already looks like {NEW_TILES_PER_METATILE}-tile-slot format")
        else:
            print(f"[SKIP] {name}: metatiles.bin ({metatiles_size}B) / metatile_attributes.bin ({attrs_size}B) "
                  f"don't match the expected old 8-tile-slot layout for either attribute format")
        return "skip"

    metatiles_data = metatiles_path.read_bytes()
    attrs_data = attrs_path.read_bytes()
    old_entries = struct.unpack(f"<{num_metatiles * OLD_TILES_PER_METATILE}H", metatiles_data)

    if attr_bytes == 2:
        raw_attrs = struct.unpack(f"<{num_metatiles}H", attrs_data)
    else:
        raw_attrs = struct.unpack(f"<{num_metatiles}I", attrs_data)

    new_entries = []
    for i in range(num_metatiles):
        old_tiles = old_entries[i * OLD_TILES_PER_METATILE:(i + 1) * OLD_TILES_PER_METATILE]
        layer_type = layer_type_for(fmt, raw_attrs[i])
        new_entries.extend(convert_metatile(old_tiles, layer_type))

    if not apply:
        print(f"[DRY] {name}: would convert {num_metatiles} metatile(s) ({fmt} attrs)")
        return "would-convert"

    backup = metatiles_path.with_suffix(metatiles_path.suffix + ".bak")
    if not backup.exists():
        backup.write_bytes(metatiles_data)

    metatiles_path.write_bytes(struct.pack(f"<{len(new_entries)}H", *new_entries))
    print(f"[OK] {name}: converted {num_metatiles} metatile(s) ({fmt} attrs) - "
          f"metatile_attributes.bin left untouched")
    return "converted"


def attr_bytes_guess(attrs_size, metatiles_size):
    # Only used for the "already converted" message above - best-effort guess.
    num_new = metatiles_size // NEW_METATILE_BYTES
    if num_new and attrs_size % num_new == 0:
        return attrs_size // num_new
    return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--yes", action="store_true", help="Actually write the converted files (default is a dry run)")
    args = ap.parse_args()

    if not TILESETS_DIR.is_dir():
        sys.exit(f"error: {TILESETS_DIR} not found")

    tileset_dirs = sorted((TILESETS_DIR / "primary").iterdir()) + sorted((TILESETS_DIR / "secondary").iterdir())
    tileset_dirs = [d for d in tileset_dirs if d.is_dir()]

    counts = {"converted": 0, "would-convert": 0, "skip": 0}
    for ts_dir in tileset_dirs:
        result = convert_tileset_dir(ts_dir, apply=args.yes)
        counts[result] = counts.get(result, 0) + 1

    print()
    if args.yes:
        print(f"Done: {counts['converted']} converted, {counts['skip']} skipped, out of {len(tileset_dirs)} tileset dir(s).")
    else:
        print(f"Dry run: {counts['would-convert']} would be converted, {counts['skip']} skipped, "
              f"out of {len(tileset_dirs)} tileset dir(s). Re-run with --yes to apply.")


if __name__ == "__main__":
    main()
