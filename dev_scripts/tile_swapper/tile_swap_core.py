#!/usr/bin/env python3
"""
Core logic for swapping metatile IDs in map layout blockdata (map.bin / border.bin).

Block format (see include/global.fieldmap.h):
    bits 0-9   metatile id   (MAPGRID_METATILE_ID_MASK 0x03FF)
    bits 10-11 collision     (MAPGRID_COLLISION_MASK   0x0C00)
    bits 12-15 elevation     (MAPGRID_ELEVATION_MASK   0xF000)

A swap only ever touches the metatile id bits; collision/elevation on each
block are left exactly as they were.

A "swap set" is a reusable, map-agnostic list of {from, to} metatile id pairs
(see swap_sets/*.json for an example). It says nothing about which map(s) it
applies to - that's chosen separately, in the GUI or via --maps on the CLI.
"""
import json
import shutil
import struct
from pathlib import Path

METATILE_ID_MASK = 0x03FF

ROOT = Path(__file__).resolve().parents[2]
LAYOUTS_JSON = ROOT / "data/layouts/layouts.json"


class LayoutNotFoundError(Exception):
    pass


def load_layouts():
    with open(LAYOUTS_JSON) as f:
        return json.load(f)["layouts"]


def find_layout(layouts, query):
    """Match a layout by id, name, or the folder name of its blockdata path.

    Matching is case-insensitive and tolerant of a trailing "_Layout" / "Layout"
    suffix, so "LittlerootTown", "LittlerootTown_Layout" and
    "LAYOUT_LITTLEROOT_TOWN" all resolve to the same entry.
    """
    q = query.strip().lower()
    q_stripped = q[:-len("_layout")] if q.endswith("_layout") else q

    for l in layouts:
        candidates = {
            l["id"].lower(),
            l["name"].lower(),
            Path(l["blockdata_filepath"]).parent.name.lower(),
        }
        if q in candidates or q_stripped in candidates:
            return l

    raise LayoutNotFoundError(f"No layout matches {query!r}")


def parse_tile_id(value):
    """Accept int, decimal string, or hex string ("0x1A" / "0x020")."""
    if isinstance(value, int):
        return value
    return int(str(value).strip(), 0)


# ---------- Swap sets ----------

def load_swap_set(path):
    """Load a swap set JSON file: a list of {"from": .., "to": ..} pairs.

    Returns a list of (from_id, to_id) int tuples, in file order.
    """
    with open(path) as f:
        data = json.load(f)
    if not isinstance(data, list):
        raise ValueError("Swap set file must be a JSON list of {\"from\": .., \"to\": ..} pairs")

    swaps = []
    for i, entry in enumerate(data):
        try:
            swaps.append((parse_tile_id(entry["from"]), parse_tile_id(entry["to"])))
        except (KeyError, TypeError) as e:
            raise ValueError(f"Swap set entry {i} is malformed: {entry!r}") from e
    return swaps


def save_swap_set(path, swaps):
    """Write a swap set as a list of {"from": "0xNNN", "to": "0xNNN"} objects."""
    data = [{"from": f"0x{f:03X}", "to": f"0x{t:03X}"} for f, t in swaps]
    with open(path, "w") as f:
        json.dump(data, f, indent=2)


# ---------- Applying swaps ----------

def _blockdata_paths(layout, include_border):
    paths = [ROOT / layout["blockdata_filepath"]]
    if include_border:
        paths.append(ROOT / layout["border_filepath"])
    return paths


def count_matches(layout, from_id, include_border=False):
    """Return how many blocks currently have metatile id == from_id."""
    total = 0
    for path in _blockdata_paths(layout, include_border):
        data = path.read_bytes()
        blocks = struct.unpack(f"<{len(data)//2}H", data)
        total += sum(1 for b in blocks if (b & METATILE_ID_MASK) == from_id)
    return total


def apply_swap_set(layout, swaps, include_border=False, backup=True):
    """Apply every (from_id, to_id) pair in `swaps` to a layout in one pass.

    Each block is remapped at most once (a single dict lookup against its
    original metatile id), so swap pairs never chain into each other even if
    one swap's `to` equals another swap's `from`. Returns the total number of
    blocks changed across map.bin (and border.bin if requested).
    """
    for _from_id, to_id in swaps:
        if not (0 <= to_id <= METATILE_ID_MASK):
            raise ValueError(f"to id {to_id} out of range 0-{METATILE_ID_MASK}")

    mapping = dict(swaps)
    changed = 0
    for path in _blockdata_paths(layout, include_border):
        data = path.read_bytes()
        blocks = list(struct.unpack(f"<{len(data)//2}H", data))

        local_changed = 0
        for i, b in enumerate(blocks):
            metatile = b & METATILE_ID_MASK
            if metatile in mapping:
                blocks[i] = (b & ~METATILE_ID_MASK) | mapping[metatile]
                local_changed += 1

        if local_changed:
            if backup:
                shutil.copyfile(path, path.with_suffix(path.suffix + ".bak"))
            path.write_bytes(struct.pack(f"<{len(blocks)}H", *blocks))

        changed += local_changed

    return changed


def apply_swap(layout, from_id, to_id, include_border=False, backup=True):
    """Convenience wrapper for a single from/to pair."""
    return apply_swap_set(layout, [(from_id, to_id)], include_border, backup)


def apply_to_maps(map_queries, swaps, layouts=None, include_border=False, backup=True):
    """Apply one swap set to several maps.

    Returns a list of result dicts in the same order as map_queries:
    {"map": query, "resolved": id-or-None, "changed": int, "error": str-or-None}.
    """
    if layouts is None:
        layouts = load_layouts()

    results = []
    for map_query in map_queries:
        try:
            layout = find_layout(layouts, map_query)
        except LayoutNotFoundError as e:
            results.append({"map": map_query, "resolved": None, "changed": 0, "error": str(e)})
            continue

        try:
            changed = apply_swap_set(layout, swaps, include_border, backup)
            results.append({"map": map_query, "resolved": layout["id"], "changed": changed, "error": None})
        except Exception as e:
            results.append({"map": map_query, "resolved": layout["id"], "changed": 0, "error": str(e)})

    return results
