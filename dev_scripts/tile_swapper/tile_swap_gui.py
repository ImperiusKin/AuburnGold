#!/usr/bin/env python3
"""
GUI for swapping metatile IDs in map layouts.

    python dev_scripts/tile_swapper/tile_swap_gui.py

Workflow:
  1. Load a "swap set" JSON file - a reusable, map-agnostic list of
     from -> to metatile id pairs, e.g. swap_sets/example.json:

        [
          {"from": "0x020", "to": "0x006"},
          {"from": "0x021", "to": "0x007"}
        ]

     (You can also add/remove pairs by hand in the table, and save the
     result back out as a new swap set for reuse.)

  2. Pick one or more maps from the list on the left (ctrl/shift-click for
     multiple, or type to filter).

  3. Click "Apply Swap Set to Selected Map(s)". The same ruleset is applied
     to every selected map in one go - no need to type it out per map.

A .bak copy of every file touched is written alongside it before it's changed.
"""
import sys
import tkinter as tk
from pathlib import Path
from tkinter import filedialog, messagebox, ttk

sys.path.insert(0, str(Path(__file__).resolve().parent))
import tile_swap_core as core


class TileSwapApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Mythril Tile Swapper")
        self.geometry("880x620")

        self.layouts = core.load_layouts()
        self.layout_labels = sorted(
            f'{l["id"]}  ({Path(l["blockdata_filepath"]).parent.name})' for l in self.layouts
        )
        self.swaps = []  # list of (from_id, to_id)

        self._build_ui()

    def _log(self, msg):
        self.log.configure(state="normal")
        self.log.insert("end", msg + "\n")
        self.log.see("end")
        self.log.configure(state="disabled")

    # ---------- Layout ----------

    def _build_ui(self):
        main = ttk.Frame(self)
        main.pack(fill="both", expand=True, padx=8, pady=8)

        # Left: map picker
        left = ttk.LabelFrame(main, text="1. Pick map(s)")
        left.pack(side="left", fill="both", expand=False, padx=(0, 8))

        self.map_filter_var = tk.StringVar()
        filter_entry = ttk.Entry(left, textvariable=self.map_filter_var, width=32)
        filter_entry.pack(fill="x", padx=6, pady=(6, 2))
        filter_entry.insert(0, "")
        self.map_filter_var.trace_add("write", lambda *_: self._refresh_map_list())

        list_frame = ttk.Frame(left)
        list_frame.pack(fill="both", expand=True, padx=6, pady=(0, 6))
        scrollbar = ttk.Scrollbar(list_frame, orient="vertical")
        self.map_listbox = tk.Listbox(
            list_frame, selectmode="extended", width=40, height=26, yscrollcommand=scrollbar.set
        )
        scrollbar.config(command=self.map_listbox.yview)
        self.map_listbox.pack(side="left", fill="both", expand=True)
        scrollbar.pack(side="right", fill="y")
        self._refresh_map_list()

        select_btns = ttk.Frame(left)
        select_btns.pack(fill="x", padx=6, pady=(0, 6))
        ttk.Button(select_btns, text="Select All Shown", command=self._select_all_shown).pack(side="left")
        ttk.Button(select_btns, text="Clear Selection", command=lambda: self.map_listbox.selection_clear(0, "end")).pack(
            side="left", padx=4
        )

        # Right: swap set + options + actions
        right = ttk.Frame(main)
        right.pack(side="left", fill="both", expand=True)

        swap_frame = ttk.LabelFrame(right, text="2. Swap set (from -> to metatile ID)")
        swap_frame.pack(fill="both", expand=True)

        swap_btns = ttk.Frame(swap_frame)
        swap_btns.pack(fill="x", padx=6, pady=6)
        ttk.Button(swap_btns, text="Load Swap Set...", command=self._load_swap_set).pack(side="left")
        ttk.Button(swap_btns, text="Save Swap Set As...", command=self._save_swap_set).pack(side="left", padx=4)
        ttk.Button(swap_btns, text="Clear", command=self._clear_swaps).pack(side="left", padx=4)

        self.swap_tree = ttk.Treeview(swap_frame, columns=("from", "to"), show="headings", height=10)
        self.swap_tree.heading("from", text="From")
        self.swap_tree.heading("to", text="To")
        self.swap_tree.column("from", width=100, anchor="center")
        self.swap_tree.column("to", width=100, anchor="center")
        self.swap_tree.pack(fill="both", expand=True, padx=6)

        add_frame = ttk.Frame(swap_frame)
        add_frame.pack(fill="x", padx=6, pady=6)
        ttk.Label(add_frame, text="From:").pack(side="left")
        self.add_from_var = tk.StringVar()
        ttk.Entry(add_frame, textvariable=self.add_from_var, width=10).pack(side="left", padx=(2, 8))
        ttk.Label(add_frame, text="To:").pack(side="left")
        self.add_to_var = tk.StringVar()
        ttk.Entry(add_frame, textvariable=self.add_to_var, width=10).pack(side="left", padx=(2, 8))
        ttk.Button(add_frame, text="Add Pair", command=self._add_pair).pack(side="left")
        ttk.Button(add_frame, text="Remove Selected", command=self._remove_selected_pair).pack(side="left", padx=4)

        # Options + apply
        opts_frame = ttk.LabelFrame(right, text="3. Apply")
        opts_frame.pack(fill="x", pady=(8, 0))

        self.border_var = tk.BooleanVar(value=False)
        ttk.Checkbutton(opts_frame, text="Also update border.bin", variable=self.border_var).pack(
            anchor="w", padx=6, pady=(6, 0)
        )
        self.backup_var = tk.BooleanVar(value=True)
        ttk.Checkbutton(opts_frame, text="Write .bak backup before changing files", variable=self.backup_var).pack(
            anchor="w", padx=6
        )

        apply_btns = ttk.Frame(opts_frame)
        apply_btns.pack(fill="x", padx=6, pady=6)
        ttk.Button(apply_btns, text="Preview Counts", command=self._preview).pack(side="left")
        ttk.Button(apply_btns, text="Apply Swap Set to Selected Map(s)", command=self._apply).pack(
            side="left", padx=8
        )

        # Log
        log_frame = ttk.LabelFrame(self, text="Log")
        log_frame.pack(fill="both", expand=False, padx=8, pady=(0, 8))
        self.log = tk.Text(log_frame, height=9, state="disabled", wrap="word")
        self.log.pack(fill="both", expand=True, padx=4, pady=4)

    # ---------- Map list ----------

    def _refresh_map_list(self):
        typed = self.map_filter_var.get().lower()
        self.map_listbox.delete(0, "end")
        self._visible_labels = [l for l in self.layout_labels if typed in l.lower()] if typed else list(self.layout_labels)
        for label in self._visible_labels:
            self.map_listbox.insert("end", label)

    def _select_all_shown(self):
        self.map_listbox.selection_set(0, "end")

    def _selected_map_queries(self):
        return [self.map_listbox.get(i).split()[0] for i in self.map_listbox.curselection()]

    # ---------- Swap set table ----------

    def _refresh_swap_tree(self):
        self.swap_tree.delete(*self.swap_tree.get_children())
        for f, t in self.swaps:
            self.swap_tree.insert("", "end", values=(f"0x{f:03X}", f"0x{t:03X}"))

    def _load_swap_set(self):
        path = filedialog.askopenfilename(
            initialdir=str(Path(__file__).resolve().parent / "swap_sets"),
            filetypes=[("JSON files", "*.json"), ("All files", "*.*")],
        )
        if not path:
            return
        try:
            self.swaps = core.load_swap_set(path)
        except Exception as e:
            messagebox.showerror("Failed to load swap set", str(e))
            return
        self._refresh_swap_tree()
        self._log(f"[swap set] Loaded {len(self.swaps)} pair(s) from {path}")

    def _save_swap_set(self):
        if not self.swaps:
            messagebox.showwarning("Nothing to save", "Add or load some swap pairs first.")
            return
        path = filedialog.asksaveasfilename(
            initialdir=str(Path(__file__).resolve().parent / "swap_sets"),
            defaultextension=".json",
            filetypes=[("JSON files", "*.json")],
        )
        if not path:
            return
        core.save_swap_set(path, self.swaps)
        self._log(f"[swap set] Saved {len(self.swaps)} pair(s) to {path}")

    def _clear_swaps(self):
        self.swaps = []
        self._refresh_swap_tree()

    def _add_pair(self):
        try:
            f = core.parse_tile_id(self.add_from_var.get())
            t = core.parse_tile_id(self.add_to_var.get())
        except Exception as e:
            messagebox.showerror("Invalid pair", str(e))
            return
        self.swaps.append((f, t))
        self._refresh_swap_tree()
        self.add_from_var.set("")
        self.add_to_var.set("")

    def _remove_selected_pair(self):
        selected = self.swap_tree.selection()
        indices = sorted((self.swap_tree.index(s) for s in selected), reverse=True)
        for i in indices:
            del self.swaps[i]
        self._refresh_swap_tree()

    # ---------- Apply ----------

    def _preview(self):
        if not self.swaps:
            messagebox.showwarning("No swap set", "Load or build a swap set first.")
            return
        queries = self._selected_map_queries()
        if not queries:
            messagebox.showwarning("No maps selected", "Select at least one map on the left.")
            return
        for query in queries:
            try:
                layout = core.find_layout(self.layouts, query)
            except core.LayoutNotFoundError as e:
                self._log(f"[preview] {query}: ERROR - {e}")
                continue
            total = sum(core.count_matches(layout, f, self.border_var.get()) for f, _t in self.swaps)
            self._log(f"[preview] {layout['id']}: {total} block(s) would be affected by {len(self.swaps)} pair(s)")

    def _apply(self):
        if not self.swaps:
            messagebox.showwarning("No swap set", "Load or build a swap set first.")
            return
        queries = self._selected_map_queries()
        if not queries:
            messagebox.showwarning("No maps selected", "Select at least one map on the left.")
            return
        if not messagebox.askyesno(
            "Confirm", f"Apply {len(self.swaps)} swap pair(s) to {len(queries)} map(s) now?"
        ):
            return

        results = core.apply_to_maps(
            queries, self.swaps, self.layouts, self.border_var.get(), self.backup_var.get()
        )
        for r in results:
            if r["error"]:
                self._log(f"[apply] {r['map']}: ERROR - {r['error']}")
            else:
                self._log(f"[apply] {r['resolved']}: {r['changed']} block(s) changed")

        errors = [r for r in results if r["error"]]
        if errors:
            messagebox.showwarning("Finished with errors", f"{len(errors)} of {len(results)} map(s) failed. See log.")
        else:
            messagebox.showinfo("Done", f"Applied swap set to {len(results)} map(s). See log for details.")


if __name__ == "__main__":
    app = TileSwapApp()
    app.mainloop()
