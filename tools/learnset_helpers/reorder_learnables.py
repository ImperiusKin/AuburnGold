#!/usr/bin/env python3

"""
One-off utility: reorder src/data/pokemon/all_learnables.json keys to follow
National Dex order (include/constants/pokedex.h), grouping alternate forms
(regional variants, mega/primal, gender/rotom/etc. forms) after all base
species instead of interleaved with their base form.

Does NOT change any move-list content, only key order. Safe to run on a
hand-edited all_learnables.json.
"""

import json
import pathlib
import re
import unicodedata


REPO_ROOT = pathlib.Path(__file__).resolve().parents[2]
POKEDEX_H = REPO_ROOT / "include/constants/pokedex.h"
LEARNABLES_JSON = REPO_ROOT / "src/data/pokemon/all_learnables.json"

MANUAL_ALIASES = {
    "FLABÉBÉ": "FLABEBE",
}


def strip_accents(s: str) -> str:
    return "".join(c for c in unicodedata.normalize("NFD", s) if unicodedata.category(c) != "Mn")


def build_dex_order() -> dict[str, int]:
    text = POKEDEX_H.read_text()
    names = re.findall(r"NATIONAL_DEX_(\w+)", text)
    dex_order = {}
    ordinal = 0
    for name in names:
        if name in ("NONE", "COUNT"):
            continue
        if name not in dex_order:
            dex_order[name] = ordinal
            ordinal += 1
    return dex_order


def find_base(key: str, dex_order: dict[str, int]) -> str:
    candidates = [b for b in dex_order if key.startswith(b + "_")]
    if not candidates:
        raise ValueError(f"Could not infer base species for alt-form key {key!r}")
    return max(candidates, key=len)


def main():
    dex_order = build_dex_order()

    with open(LEARNABLES_JSON, "r") as fp:
        data = json.load(fp)

    base_keys = []
    alt_keys = []
    for key in data:
        lookup_key = MANUAL_ALIASES.get(key, key)
        if lookup_key in dex_order:
            base_keys.append(key)
        else:
            alt_keys.append(key)

    base_keys.sort(key=lambda k: dex_order[MANUAL_ALIASES.get(k, k)])

    def alt_sort_key(key: str):
        lookup_key = MANUAL_ALIASES.get(key, key)
        base = find_base(lookup_key, dex_order)
        return (dex_order[base], key)

    alt_keys.sort(key=alt_sort_key)

    ordered = {key: data[key] for key in base_keys + alt_keys}

    assert ordered.keys() == data.keys()
    for key in data:
        assert ordered[key] == data[key]

    with open(LEARNABLES_JSON, "w") as fp:
        json.dump(ordered, fp, indent=2)

    print(f"Reordered {len(base_keys)} base species and {len(alt_keys)} alt forms.")


if __name__ == "__main__":
    main()
