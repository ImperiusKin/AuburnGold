#!/usr/bin/env python3
"""
Rename a map throughout the project: folder, layout, map id constant,
map group listing, script identifiers, and every cross-reference
(warps/connections) that points at it from other maps.

    python dev_scripts/map_renamer/rename_map.py Player_House NewBarkTown_Player_House

What it touches:
  - data/maps/<Old>/            -> data/maps/<New>/            (git mv)
  - data/layouts/<Old>/         -> data/layouts/<New>/          (git mv,
    only if that layout isn't shared with another map)
  - data/maps/<Old>/map.json    "id" / "name" / "layout" fields
  - data/layouts/layouts.json   the layout's "id" / "name" / filepaths
  - data/maps/map_groups.json   the quoted entry in its gMapGroup_* list
  - data/event_scripts.s        the two ".include" lines for this map
  - scripts.pory / scripts.inc / map.json inside the map's own folder:
    every "<Old>_..." identifier (MapScripts, EventScript_*, etc.)
  - every other map.json / .pory / .inc / .s / .c / .h in the repo:
    whole-word references to the map's MAP_ id and (if renamed) the
    layout's LAYOUT_ id constant (warp dest_map, connections, etc.)

It deliberately leaves data/maps/<New>/connections.inc, events.inc and
header.inc alone - those are gitignored and regenerated from map.json by
`make generated` / the normal build.

Run with --dry-run first to see the full plan without touching anything.
"""
import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
MAPS_DIR = ROOT / "data/maps"
LAYOUTS_DIR = ROOT / "data/layouts"
LAYOUTS_JSON = LAYOUTS_DIR / "layouts.json"
MAP_GROUPS_JSON = MAPS_DIR / "map_groups.json"
EVENT_SCRIPTS_S = ROOT / "data/event_scripts.s"

# Extensions scanned repo-wide for cross-references to the renamed
# map/layout id constants (warp dest_map, connections, asm/.pory refs, ...).
SCAN_EXTS = {".json", ".pory", ".inc", ".s", ".c", ".h"}
SCAN_EXCLUDE_DIRS = {".git", "build", "tools", "dev_scripts", "__pycache__"}


def camel_words(s):
    return re.findall(r"[A-Z]+(?![a-z])|[A-Z][a-z]*|[a-z0-9]+", s)


def to_constant_suffix(name):
    """'NewBarkTown_Player_House' -> 'NEW_BARK_TOWN_PLAYER_HOUSE'"""
    words = []
    for part in name.split("_"):
        words.extend(camel_words(part))
    return "_".join(w.upper() for w in words if w)


def word_replace(text, old, new):
    return re.sub(r"\b" + re.escape(old) + r"\b", new, text)


def prefix_replace(text, old, new):
    """Like word_replace, but also matches when followed by '_' - so it
    catches 'Old_EventScript_Foo' too, not just a bare standalone 'Old'
    (word boundaries alone treat '_' as a word char, so \\b would miss
    that case)."""
    return re.sub(r"\b" + re.escape(old) + r"(?=_|\b)", new, text)


def run(cmd, dry_run):
    print("  $ " + " ".join(str(c) for c in cmd))
    if not dry_run:
        subprocess.run(cmd, cwd=ROOT, check=True)


def git_mv(src: Path, dst: Path, dry_run):
    run(["git", "mv", str(src.relative_to(ROOT)), str(dst.relative_to(ROOT))], dry_run)


def iter_scan_files():
    candidates = []
    for path in ROOT.rglob("*"):
        if not path.is_file() or path.suffix not in SCAN_EXTS:
            continue
        if any(part in SCAN_EXCLUDE_DIRS for part in path.relative_to(ROOT).parts):
            continue
        candidates.append(path)

    # Skip gitignored files (connections.inc/events.inc/header.inc, the
    # generated include/constants/*.h, ...) - they're regenerated from the
    # sources we do edit, so touching them too is just noise.
    if not candidates:
        return
    result = subprocess.run(
        ["git", "check-ignore", "--stdin"],
        cwd=ROOT, input="\n".join(str(p.relative_to(ROOT)) for p in candidates),
        capture_output=True, text=True,
    )
    ignored = set(result.stdout.splitlines())
    for path in candidates:
        if str(path.relative_to(ROOT)) not in ignored:
            yield path


def apply_word_replacements(path: Path, replacements, dry_run):
    text = path.read_text()
    new_text = text
    for old, new, *mode in replacements:
        fn = prefix_replace if mode and mode[0] == "prefix" else word_replace
        new_text = fn(new_text, old, new)
    if new_text != text:
        print(f"  updating {path.relative_to(ROOT)}")
        if not dry_run:
            path.write_text(new_text)
        return True
    return False


def count_layout_refs(layout_id):
    count = 0
    for map_json in MAPS_DIR.glob("*/map.json"):
        text = map_json.read_text()
        m = re.search(r'"layout":\s*"([^"]+)"', text)
        if m and m.group(1) == layout_id:
            count += 1
    return count


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("old_name", help="current map folder/name, e.g. Player_House")
    ap.add_argument("new_name", help="new map folder/name, e.g. NewBarkTown_Player_House")
    ap.add_argument("--no-layout-rename", action="store_true",
                     help="keep the existing layout id/folder even if it's only used by this map")
    ap.add_argument("--dry-run", action="store_true", help="print the plan, change nothing")
    args = ap.parse_args()

    old_name, new_name = args.old_name, args.new_name
    old_dir = MAPS_DIR / old_name
    new_dir = MAPS_DIR / new_name

    if not old_dir.is_dir():
        sys.exit(f"error: {old_dir.relative_to(ROOT)} does not exist")
    if new_dir.exists():
        sys.exit(f"error: {new_dir.relative_to(ROOT)} already exists")

    old_map_json_text = (old_dir / "map.json").read_text()
    m = re.search(r'"id":\s*"([^"]+)"', old_map_json_text)
    if not m:
        sys.exit(f"error: couldn't find \"id\" field in {old_dir/'map.json'}")
    old_map_id = m.group(1)
    new_map_id = "MAP_" + to_constant_suffix(new_name)

    m = re.search(r'"layout":\s*"([^"]+)"', old_map_json_text)
    if not m:
        sys.exit(f"error: couldn't find \"layout\" field in {old_dir/'map.json'}")
    old_layout_id = m.group(1)

    rename_layout = False
    old_layout_dir = None
    new_layout_id = old_layout_id
    if not args.no_layout_rename:
        layout_text = LAYOUTS_JSON.read_text()
        lm = re.search(
            r'\{\s*"id":\s*"' + re.escape(old_layout_id) + r'".*?"blockdata_filepath":\s*"([^"]+)"',
            layout_text, re.S,
        )
        if lm:
            old_layout_dir = (ROOT / lm.group(1)).parent
            refs = count_layout_refs(old_layout_id)
            if refs <= 1 and old_layout_dir.is_dir():
                rename_layout = True
                new_layout_id = "LAYOUT_" + to_constant_suffix(new_name)
            elif refs > 1:
                print(f"note: {old_layout_id} is used by {refs} maps, leaving the layout untouched")

    print(f"Renaming map {old_name!r} -> {new_name!r}")
    print(f"  {old_map_id} -> {new_map_id}")
    if rename_layout:
        new_layout_dir = LAYOUTS_DIR / new_name
        print(f"  {old_layout_id} -> {new_layout_id}")
        print(f"  {old_layout_dir.relative_to(ROOT)} -> {new_layout_dir.relative_to(ROOT)}")
    print()

    dry_run = args.dry_run

    # 1. Move the map folder, then the layout folder (if applicable).
    git_mv(old_dir, new_dir, dry_run)
    if rename_layout:
        git_mv(old_layout_dir, LAYOUTS_DIR / new_name, dry_run)

    # Files physically stay put during --dry-run (git mv is only printed,
    # not run), so read/write through whichever path actually has them.
    map_dir_now = old_dir if dry_run else new_dir

    # 2. Rename every "<old_name>_..." identifier inside the map's own files
    #    (MapScripts, EventScript_*, ...) plus the id/name/layout fields.
    local_replacements = [(old_name, new_name, "prefix"), (old_map_id, new_map_id)]
    if rename_layout:
        local_replacements.append((old_layout_id, new_layout_id))
    for fname in ("map.json", "scripts.pory", "scripts.inc"):
        fpath = map_dir_now / fname
        if fpath.is_file():
            apply_word_replacements(fpath, local_replacements, dry_run)

    # 3. Update the layout's own entry in layouts.json.
    if rename_layout:
        text = LAYOUTS_JSON.read_text()
        old_layout_name_m = re.search(
            r'"id":\s*"' + re.escape(old_layout_id) + r'",\s*"name":\s*"([^"]+)"', text)
        new_text = word_replace(text, old_layout_id, new_layout_id)
        new_text = new_text.replace(
            f"{LAYOUTS_DIR.relative_to(ROOT)}/{old_name}/",
            f"{LAYOUTS_DIR.relative_to(ROOT)}/{new_name}/",
        )
        if old_layout_name_m:
            old_layout_name = old_layout_name_m.group(1)
            new_layout_name = old_layout_name.replace(old_name, new_name)
            new_text = new_text.replace(f'"{old_layout_name}"', f'"{new_layout_name}"')
        if new_text != text:
            print(f"  updating {LAYOUTS_JSON.relative_to(ROOT)}")
            if not dry_run:
                LAYOUTS_JSON.write_text(new_text)

    # 4. Update the map group listing.
    text = MAP_GROUPS_JSON.read_text()
    new_text = text.replace(f'"{old_name}"', f'"{new_name}"')
    if new_text != text:
        print(f"  updating {MAP_GROUPS_JSON.relative_to(ROOT)}")
        if not dry_run:
            MAP_GROUPS_JSON.write_text(new_text)
    else:
        print(f"warning: {old_name!r} not found as a plain list entry in {MAP_GROUPS_JSON.relative_to(ROOT)}")

    # 5. Update the .include lines pulling this map's scripts.inc in.
    text = EVENT_SCRIPTS_S.read_text()
    new_text = text.replace(f"data/maps/{old_name}/", f"data/maps/{new_name}/")
    if new_text != text:
        print(f"  updating {EVENT_SCRIPTS_S.relative_to(ROOT)}")
        if not dry_run:
            EVENT_SCRIPTS_S.write_text(new_text)

    # 6. Fix up every other file's references to the id constants
    #    (other maps' warps/connections, any hardcoded asm/C refs, ...).
    global_replacements = [(old_map_id, new_map_id)]
    if rename_layout:
        global_replacements.append((old_layout_id, new_layout_id))
    for path in iter_scan_files():
        if path.is_relative_to(map_dir_now):
            continue  # already handled above
        apply_word_replacements(path, global_replacements, dry_run)

    print()
    if dry_run:
        print("Dry run only - nothing was changed. Re-run without --dry-run to apply.")
    else:
        print("Done. Run `make generated` (or a normal build) to regenerate")
        print("map_groups.h, layout headers, and this map's connections/events/header.inc.")


if __name__ == "__main__":
    main()
