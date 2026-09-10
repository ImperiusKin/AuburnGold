#!/usr/bin/env python3
"""
GUI front-end for tileset_fixer.py.

    python3 dev_scripts/tileset_fixer/tileset_fixer_gui.py

Pick a tileset on the left, read its detail panel, then run one of the
actions on the right - each one is the same function tileset_fixer.py's
CLI calls, just wrapped in a confirm dialog instead of a "type yes" prompt.

Every mutating action still writes a `.bak` next to each file it touches
the first time (tileset_fixer.py's own doing) - use "Restore Backups for
This Tileset" here to put them back, instead of the CLI's interactive
restore-prompt (which needs a terminal this GUI doesn't have).
"""
import contextlib
import io
import sys
import threading
import tkinter as tk
from pathlib import Path
from tkinter import messagebox, simpledialog, ttk

sys.path.insert(0, str(Path(__file__).resolve().parent))
import tileset_fixer as core


class TilesetFixerApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("AuburnGold Tileset Fixer")
        self.geometry("1000x660")

        self.tilesets = {}
        self.names_shown = []
        self.selected_name = None

        self._build_ui()
        self._reload()

    # ---------- Data ----------

    def _reload(self):
        incbin_paths = core.parse_incbin_paths()
        tiles_paths, palette_paths = core.parse_graphics_paths()
        self.tilesets = core.parse_tilesets(incbin_paths, tiles_paths, palette_paths)
        core.attach_map_usage(self.tilesets)
        self._refresh_list()

    def _resolve_primary(self, ts):
        paired = core.find_paired_primaries(ts)
        if len(paired) != 1:
            messagebox.showerror(
                "Ambiguous pairing",
                f"{ts.name} is paired with {len(paired)} different primaries "
                f"({sorted(p for p in paired if p)}); this action only supports "
                f"an unambiguous 1:1 pairing.",
            )
            return None
        primary_ts = self.tilesets.get(next(iter(paired)))
        if not primary_ts:
            messagebox.showerror("Error", "Could not resolve the paired primary tileset.")
            return None
        return primary_ts

    # ---------- Layout ----------

    def _build_ui(self):
        main = ttk.Frame(self)
        main.pack(fill="both", expand=True, padx=8, pady=8)

        left = ttk.LabelFrame(main, text="Tilesets")
        left.pack(side="left", fill="y", padx=(0, 8))

        self.filter_var = tk.StringVar()
        entry = ttk.Entry(left, textvariable=self.filter_var, width=44)
        entry.pack(fill="x", padx=6, pady=(6, 2))
        self.filter_var.trace_add("write", lambda *_: self._refresh_list())

        list_frame = ttk.Frame(left)
        list_frame.pack(fill="both", expand=True, padx=6, pady=(0, 6))
        scrollbar = ttk.Scrollbar(list_frame, orient="vertical")
        self.listbox = tk.Listbox(
            list_frame, width=54, height=34, exportselection=False, yscrollcommand=scrollbar.set
        )
        scrollbar.config(command=self.listbox.yview)
        self.listbox.pack(side="left", fill="both", expand=True)
        scrollbar.pack(side="right", fill="y")
        self.listbox.bind("<<ListboxSelect>>", lambda e: self._on_select())

        ttk.Button(left, text="Reload From Disk", command=self._reload).pack(fill="x", padx=6, pady=(0, 6))

        right = ttk.Frame(main)
        right.pack(side="left", fill="both", expand=True)

        self.detail_var = tk.StringVar(value="Select a tileset on the left.")
        ttk.Label(right, textvariable=self.detail_var, justify="left", wraplength=620).pack(fill="x", pady=(0, 8))

        actions = ttk.LabelFrame(right, text="Actions")
        actions.pack(fill="x")
        row1 = ttk.Frame(actions)
        row1.pack(fill="x", padx=6, pady=(6, 3))
        row2 = ttk.Frame(actions)
        row2.pack(fill="x", padx=6, pady=(0, 3))
        row3 = ttk.Frame(actions)
        row3.pack(fill="x", padx=6, pady=(0, 6))

        self.buttons = {}

        def add(row, label, command, key):
            b = ttk.Button(row, text=label, command=command)
            b.pack(side="left", padx=(0, 6))
            self.buttons[key] = b

        add(row1, "Wipe Metatile Data", self._wipe, "wipe")
        add(row1, "Deduplicate Tiles", self._dedupe, "dedupe")
        add(row1, "Prune Unused", self._prune, "prune")
        add(row2, "Make Standalone...", self._make_standalone, "standalone")
        add(row2, "Analyze Palette Usage", self._analyze_palettes, "analyze")
        add(row3, "Consolidate Palettes", self._consolidate_palettes, "consolidate")
        add(row3, "Minimize Own Palettes", self._minimize_palettes, "minimize")
        ttk.Button(actions, text="Restore Backups for This Tileset", command=self._restore_backups).pack(
            anchor="w", padx=6, pady=(0, 6)
        )

        log_frame = ttk.LabelFrame(right, text="Output")
        log_frame.pack(fill="both", expand=True, pady=(8, 0))
        self.log = tk.Text(log_frame, state="disabled", wrap="word", height=20)
        log_scroll = ttk.Scrollbar(log_frame, orient="vertical", command=self.log.yview)
        self.log.configure(yscrollcommand=log_scroll.set)
        self.log.pack(side="left", fill="both", expand=True, padx=(6, 0), pady=6)
        log_scroll.pack(side="right", fill="y", pady=6)

        self._set_actions_enabled(False)

    def _set_actions_enabled(self, enabled, secondary_only_enabled=None):
        if secondary_only_enabled is None:
            secondary_only_enabled = enabled
        state = "normal" if enabled else "disabled"
        sec_state = "normal" if secondary_only_enabled else "disabled"
        for key in ("wipe", "dedupe", "prune"):
            self.buttons[key].configure(state=state)
        for key in ("standalone", "analyze", "consolidate", "minimize"):
            self.buttons[key].configure(state=sec_state)

    # ---------- List / selection ----------

    def _refresh_list(self):
        needle = self.filter_var.get().lower()
        names = sorted(self.tilesets.keys())
        if needle:
            names = [n for n in names if needle in n.lower()]
        self.names_shown = names
        self.listbox.delete(0, "end")
        for name in names:
            ts = self.tilesets[name]
            role = "sec" if ts.is_secondary else "prim"
            fmt, _ = core.detect_format(ts)
            self.listbox.insert("end", f"[{role:4}] {name:<38} {fmt or '?':<8} {len(ts.maps)} map(s)")

    def _on_select(self):
        sel = self.listbox.curselection()
        if not sel:
            return
        name = self.names_shown[sel[0]]
        self.selected_name = name
        ts = self.tilesets[name]
        fmt, _ = core.detect_format(ts)
        role = "secondary" if ts.is_secondary else "primary"
        lines = [
            f"{ts.name}  ({role}, {fmt} format)",
            f"dir: {ts.dir.relative_to(core.ROOT) if ts.dir else '?'}",
        ]
        if ts.maps:
            lines.append(f"used by {len(ts.maps)} map(s): " + ", ".join(sorted({m for m, _ in ts.maps}))[:400])
        else:
            lines.append("used by 0 maps (not referenced by any layout)")
        self.detail_var.set("\n".join(lines))
        self._set_actions_enabled(True, secondary_only_enabled=ts.is_secondary)

    def _current_ts(self):
        if not self.selected_name:
            return None
        return self.tilesets.get(self.selected_name)

    # ---------- Running actions ----------

    def _log(self, text):
        self.log.configure(state="normal")
        self.log.insert("end", text)
        self.log.see("end")
        self.log.configure(state="disabled")

    def _run_in_background(self, fn):
        self._set_actions_enabled(False)
        self.log.configure(state="normal")
        self.log.delete("1.0", "end")
        self.log.configure(state="disabled")

        def worker():
            buf = io.StringIO()
            try:
                with contextlib.redirect_stdout(buf):
                    fn()
            except Exception as exc:
                buf.write(f"\nerror: {exc}\n")
            finally:
                output = buf.getvalue()
                self.after(0, self._log, output)
                self.after(0, self._reload)
                self.after(0, lambda: self.buttons and self._reselect())

        threading.Thread(target=worker, daemon=True).start()

    def _reselect(self):
        # After a reload, the tileset list is rebuilt - try to keep the same one selected.
        if self.selected_name in self.names_shown:
            idx = self.names_shown.index(self.selected_name)
            self.listbox.selection_set(idx)
            self._on_select()
        else:
            self._set_actions_enabled(False)

    def _confirm(self, message):
        return messagebox.askyesno("Confirm", message)

    # ---------- Actions ----------

    def _wipe(self):
        ts = self._current_ts()
        if not ts:
            return
        if not self._confirm(
            f"This will overwrite {ts.attrs_path.relative_to(core.ROOT)} "
            f"(affects {len(ts.maps)} map(s)). Continue?"
        ):
            return
        self._run_in_background(lambda: core.wipe_attributes(ts, ask_restore=False))

    def _dedupe(self):
        ts = self._current_ts()
        if not ts:
            return
        if not self._confirm(
            f"This will overwrite {ts.dir.relative_to(core.ROOT)}/tiles.png and "
            f"{ts.metatiles_path.relative_to(core.ROOT)} (affects {len(ts.maps)} map(s)). Continue?"
        ):
            return
        self._run_in_background(lambda: core.dedupe_tileset(ts, ask_restore=False))

    def _prune(self):
        ts = self._current_ts()
        if not ts:
            return
        if not self._confirm(
            f"This will overwrite {ts.metatiles_path.relative_to(core.ROOT)}, "
            f"{ts.attrs_path.relative_to(core.ROOT)} and {ts.dir.relative_to(core.ROOT)}/tiles.png "
            f"(affects {len(ts.maps)} map(s)). Continue?"
        ):
            return
        self._run_in_background(lambda: core.prune_unused(ts, self.tilesets, ask_restore=False))

    def _make_standalone(self):
        ts = self._current_ts()
        if not ts or not ts.is_secondary:
            return
        primary_ts = self._resolve_primary(ts)
        if not primary_ts:
            return
        default_dir = ts.dir.name + "_merged"
        new_dir_name = simpledialog.askstring(
            "New tileset directory name", "Directory name for the new standalone tileset:",
            initialvalue=default_dir, parent=self,
        )
        if not new_dir_name:
            return
        new_symbol = core._new_c_symbol(new_dir_name)
        if new_symbol in self.tilesets:
            messagebox.showerror("Error", f"{new_symbol} already exists.")
            return
        if not self._confirm(
            f"This will create a new tileset directory, register it in headers.h/"
            f"metatiles.h/graphics.h, and update layouts.json (leaving {ts.name} "
            f"untouched). Continue?"
        ):
            return

        def do_merge():
            new_dir, pal_count = core.merge_secondary_absorbing_primary(primary_ts, ts, new_dir_name)
            core.register_tileset_c_source(new_symbol, new_dir_name, ts.is_compressed, pal_count)
            core.repoint_layouts(ts.name, primary_ts.name, new_symbol)
            print(f"\n{ts.name} and its files were left untouched - "
                  f"delete them once you've verified {new_symbol} in-game.")

        self._run_in_background(do_merge)

    def _analyze_palettes(self):
        ts = self._current_ts()
        if not ts or not ts.is_secondary:
            return
        primary_ts = self._resolve_primary(ts)
        if not primary_ts:
            return
        self._run_in_background(lambda: core.analyze_palette_usage(primary_ts, ts))

    def _consolidate_palettes(self):
        ts = self._current_ts()
        if not ts or not ts.is_secondary:
            return
        primary_ts = self._resolve_primary(ts)
        if not primary_ts:
            return
        if not self._confirm(
            f"This will overwrite {ts.metatiles_path.relative_to(core.ROOT)} and "
            f"{ts.dir.relative_to(core.ROOT)}/palettes/*.pal (affects {len(ts.maps)} map(s)). "
            f"{primary_ts.name}'s own files are never touched. Continue?"
        ):
            return
        self._run_in_background(lambda: core.apply_palette_consolidation(primary_ts, ts, ask_restore=False))

    def _minimize_palettes(self):
        ts = self._current_ts()
        if not ts or not ts.is_secondary:
            return
        primary_ts = self._resolve_primary(ts)
        if not primary_ts:
            return
        if not self._confirm(
            f"This will overwrite {ts.metatiles_path.relative_to(core.ROOT)} and "
            f"{ts.dir.relative_to(core.ROOT)}/palettes/*.pal (affects {len(ts.maps)} map(s)). "
            f"{primary_ts.name}'s own files are never touched. Continue?"
        ):
            return
        self._run_in_background(lambda: core.apply_secondary_palette_minimization(primary_ts, ts, ask_restore=False))

    def _restore_backups(self):
        ts = self._current_ts()
        if not ts or not ts.dir:
            return
        backups = sorted(ts.dir.rglob("*.bak"))
        if not backups:
            messagebox.showinfo("Restore Backups", f"No .bak files found under {ts.dir.relative_to(core.ROOT)}/.")
            return
        names = "\n".join(str(b.relative_to(core.ROOT)) for b in backups)
        if not self._confirm(f"Restore {len(backups)} file(s) from backup?\n\n{names}"):
            return

        def do_restore():
            for bak in backups:
                target = bak.with_name(bak.name[:-len(".bak")])
                target.write_bytes(bak.read_bytes())
                bak.unlink()
                print(f"  Restored {target.relative_to(core.ROOT)}")

        self._run_in_background(do_restore)


if __name__ == "__main__":
    TilesetFixerApp().mainloop()
