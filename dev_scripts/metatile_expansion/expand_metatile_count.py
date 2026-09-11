#!/usr/bin/env python3
"""
Implements https://github.com/pret/pokeemerald/wiki/Expanding-The-Metatile-Count
for this fork: raises the metatile ID field in the map grid from 10 to 12
bits (1024 -> 4096 metatiles total), at the cost of shrinking elevation
from 4 bits (16 levels) to 3 (8 levels) and collision from 2 bits to 1
(the reference implementation, and a check of every MapGridGetCollisionAt
consumer in this repo, confirm the 2nd collision bit was never used).

Because this fork keeps SEPARATE primary/secondary boundaries for
"emerald"-format (2-byte attrs) and "frlg"-format (4-byte attrs) tilesets -
something the upstream reference implementation doesn't have to deal with -
the boundary used for any given map/label is resolved per-tileset via the
same attribute-byte-size detection dev_scripts/tileset_fixer/ already uses,
not hardcoded to a single old/new pair like the reference script.

    OLD_PRIMARY = {emerald: 512,  frlg: 640}   -> NEW_PRIMARY = {emerald: 2048, frlg: 2560}
    OLD_TOTAL   = 1024                          -> NEW_TOTAL   = 4096

Usage:
    python3 dev_scripts/metatile_expansion/expand_metatile_count.py --labels [--yes]
    python3 dev_scripts/metatile_expansion/expand_metatile_count.py --maps [--yes]
    python3 dev_scripts/metatile_expansion/expand_metatile_count.py --all [--yes]

Run without --yes first (the default) to see the full plan, including any
elevation values in 7-14 that can't survive the 16->8 level shrink cleanly
(only 0 and old-15/ELEVATION_MULTI_LEVEL have a defined new meaning) and any
metatile_labels.h entries this script can't confidently resolve to a
tileset (left untouched either way - reported so you can fix them by hand).

Still needed after running both steps (this script does NOT do these):
  - include/fieldmap.h: NUM_METATILES_IN_PRIMARY(_FRLG)/NUM_METATILES_TOTAL
  - include/global.fieldmap.h: MAPGRID_* masks/shifts, ELEVATION_MULTI_LEVEL
  - porymap.project.cfg / a new collisions.png for porymap's collision view
"""
import argparse
import json
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "dev_scripts/tileset_fixer"))
import tileset_fixer as core  # noqa: E402

METATILE_LABELS_H = ROOT / "include/constants/metatile_labels.h"
LAYOUTS_JSON = ROOT / "data/layouts/layouts.json"

OLD_PRIMARY = {"emerald": 512, "frlg": 640}
NEW_PRIMARY = {"emerald": 2048, "frlg": 2560}
NEW_TOTAL = 4096

OLD_METATILE_MASK = 0x03FF
OLD_COLLISION_MASK = 0x0C00
OLD_COLLISION_SHIFT = 10
OLD_ELEVATION_MASK = 0xF000
OLD_ELEVATION_SHIFT = 12

NEW_COLLISION_SHIFT = 12
NEW_ELEVATION_SHIFT = 13
OLD_MULTI_LEVEL = 15
NEW_MULTI_LEVEL = 7  # matches ELEVATION_MULTI_LEVEL after the global.fieldmap.h edit

LABEL_DEFINE_RE = re.compile(r'^(#define\s+METATILE_\S+\s+)0x([0-9A-Fa-f]+)(.*)$')
SECTION_RE = re.compile(r'^// (gTileset_\S+)\s*$')


def load_tilesets():
    incbin_paths = core.parse_incbin_paths()
    tiles_paths, palette_paths = core.parse_graphics_paths()
    tilesets = core.parse_tilesets(incbin_paths, tiles_paths, palette_paths)
    return tilesets


def shift_for(ts):
    """Returns the amount to add to a metatile id owned by this tileset if
    it's a secondary (0 for a primary - those never need shifting, their
    values already sit below whichever boundary applies)."""
    if not ts.is_secondary:
        return 0
    fmt, _ = core.detect_format(ts)
    if fmt not in OLD_PRIMARY:
        return None  # unknown format - can't safely resolve
    return NEW_PRIMARY[fmt] - OLD_PRIMARY[fmt]


def remap_metatile_labels(tilesets, apply: bool):
    text = METATILE_LABELS_H.read_text()
    lines = text.splitlines(keepends=True)

    current_ts = None
    current_shift = 0
    unresolved_section = False
    changed = 0
    unresolved_labels = []
    out_lines = []

    for line in lines:
        sm = SECTION_RE.match(line.rstrip("\n"))
        if sm:
            name = sm.group(1)
            ts = tilesets.get(name)
            if ts is None:
                current_ts = None
                unresolved_section = True
                current_shift = 0
            else:
                current_ts = ts
                unresolved_section = False
                current_shift = shift_for(ts)
                if current_shift is None:
                    unresolved_section = True
                    current_shift = 0
            out_lines.append(line)
            continue

        if line.strip() == "// Other":
            current_ts = None
            unresolved_section = True
            current_shift = 0
            out_lines.append(line)
            continue

        dm = LABEL_DEFINE_RE.match(line.rstrip("\n"))
        if dm and not unresolved_section and current_shift:
            prefix, hexval, suffix = dm.groups()
            old_val = int(hexval, 16)
            new_val = old_val + current_shift
            out_lines.append(f"{prefix}0x{new_val:X}{suffix}\n")
            changed += 1
            continue

        if dm and unresolved_section:
            unresolved_labels.append(line.strip())

        out_lines.append(line)

    if unresolved_labels:
        print(f"[WARN] {len(unresolved_labels)} label(s) left UNCHANGED - couldn't confidently "
              f"resolve their tileset (review by hand, likely in the trailing \"// Other\" section):")
        for l in unresolved_labels:
            print(f"    {l}")

    print(f"metatile_labels.h: {'would shift' if not apply else 'shifted'} {changed} label(s)")
    if apply and (changed or unresolved_labels):
        METATILE_LABELS_H.write_text("".join(out_lines))


def read_u16_array(path: Path):
    data = path.read_bytes()
    n = len(data) // 2
    return list(struct.unpack(f"<{n}H", data))


def write_u16_array(path: Path, values):
    path.write_bytes(struct.pack(f"<{len(values)}H", *values))


def remap_map_data(apply: bool):
    layouts = json.loads(LAYOUTS_JSON.read_text())["layouts"]

    seen_files = {}
    conflicts = []
    for layout in layouts:
        fmt = "frlg" if layout.get("layout_version") == "frlg" else "emerald"
        for key in ("blockdata_filepath", "border_filepath"):
            rel = layout.get(key)
            if not rel:
                continue
            path = ROOT / rel
            if path in seen_files and seen_files[path] != fmt:
                conflicts.append((path, seen_files[path], fmt))
            seen_files.setdefault(path, fmt)

    if conflicts:
        print(f"[WARN] {len(conflicts)} file(s) claimed by layouts of different formats - skipping them:")
        for path, f1, f2 in conflicts:
            print(f"    {path.relative_to(ROOT)}: {f1} vs {f2}")
    conflict_paths = {c[0] for c in conflicts}

    anomalies = []  # (path, index, old_elevation)
    map_count = 0
    border_count = 0

    for layout in layouts:
        fmt = "frlg" if layout.get("layout_version") == "frlg" else "emerald"
        old_boundary = OLD_PRIMARY[fmt]
        new_boundary = NEW_PRIMARY[fmt]
        shift = new_boundary - old_boundary

        blockdata_rel = layout.get("blockdata_filepath")
        if blockdata_rel:
            path = ROOT / blockdata_rel
            if path.is_file() and path not in conflict_paths:
                values = read_u16_array(path)
                new_values = []
                for i, v in enumerate(values):
                    metatile_id = v & OLD_METATILE_MASK
                    collision = (v & OLD_COLLISION_MASK) >> OLD_COLLISION_SHIFT
                    elevation = (v & OLD_ELEVATION_MASK) >> OLD_ELEVATION_SHIFT
                    if elevation == OLD_MULTI_LEVEL:
                        elevation = NEW_MULTI_LEVEL
                    elif elevation > NEW_MULTI_LEVEL:
                        anomalies.append((path, i, elevation))
                        elevation = NEW_MULTI_LEVEL  # safest fallback: treat as "ignore" rather than truncate
                    if metatile_id >= old_boundary:
                        metatile_id += shift
                    new_values.append(metatile_id | (collision << NEW_COLLISION_SHIFT) | (elevation << NEW_ELEVATION_SHIFT))
                if apply:
                    backup = path.with_suffix(path.suffix + ".bak")
                    if not backup.exists():
                        backup.write_bytes(path.read_bytes())
                    write_u16_array(path, new_values)
                map_count += 1

        border_rel = layout.get("border_filepath")
        if border_rel:
            path = ROOT / border_rel
            if path.is_file() and path not in conflict_paths:
                values = read_u16_array(path)
                new_values = []
                for v in values:
                    metatile_id = v & OLD_METATILE_MASK
                    if metatile_id >= old_boundary:
                        metatile_id += shift
                    new_values.append(metatile_id)
                if apply:
                    backup = path.with_suffix(path.suffix + ".bak")
                    if not backup.exists():
                        backup.write_bytes(path.read_bytes())
                    write_u16_array(path, new_values)
                border_count += 1

    verb = "converted" if apply else "would convert"
    print(f"map.bin: {verb} {map_count} file(s)")
    print(f"border.bin: {verb} {border_count} file(s)")

    if anomalies:
        print(f"\n[WARN] {len(anomalies)} cell(s) had an elevation of 7-14, which has no clean "
              f"equivalent in the new 3-bit (0-7) field - clamped to {NEW_MULTI_LEVEL} "
              f"(ELEVATION_MULTI_LEVEL/\"ignore\") rather than silently truncated. Review these:")
        by_file = {}
        for path, i, old_elev in anomalies:
            by_file.setdefault(path, []).append((i, old_elev))
        for path, entries in by_file.items():
            print(f"    {path.relative_to(ROOT)}: {len(entries)} cell(s), old elevation(s) "
                  f"{sorted({e for _, e in entries})}")
    else:
        print("\nNo elevation values in the 7-14 range found - the 16->8 level shrink is clean.")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--labels", action="store_true", help="Remap include/constants/metatile_labels.h")
    ap.add_argument("--maps", action="store_true", help="Remap every layout's map.bin and border.bin")
    ap.add_argument("--all", action="store_true", help="Both --labels and --maps")
    ap.add_argument("--yes", action="store_true", help="Actually write changes (default is a dry run/report)")
    args = ap.parse_args()

    if not (args.labels or args.maps or args.all):
        ap.error("pass --labels, --maps, or --all")

    if args.labels or args.all:
        tilesets = load_tilesets()
        remap_metatile_labels(tilesets, apply=args.yes)
        print()
    if args.maps or args.all:
        remap_map_data(apply=args.yes)

    if not args.yes:
        print("\nDry run only - nothing was changed. Re-run with --yes to apply.")


if __name__ == "__main__":
    main()
