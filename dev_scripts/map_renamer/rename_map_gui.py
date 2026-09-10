#!/usr/bin/env python3
"""
GUI front-end for rename_map.py.

    python3 dev_scripts/map_renamer/rename_map_gui.py

Pick a map from the list, type the new name, hit "Dry Run" to see the full
plan, then "Rename" to actually do it. Everything rename_map.py itself does
(git mv, cross-reference fixups, etc.) is unchanged - this just wraps it in
a couple of buttons and a log window.

IMPORTANT: close porymap (or any other map editor) before renaming. Two
tools saving the same map.json/layouts.json at once will stomp on each
other's changes.
"""
import subprocess
import sys
import threading
import tkinter as tk
from pathlib import Path
from tkinter import messagebox, ttk

ROOT = Path(__file__).resolve().parents[2]
MAPS_DIR = ROOT / "data/maps"
RENAME_SCRIPT = Path(__file__).resolve().parent / "rename_map.py"


class RenameMapApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("AuburnGold Map Renamer")
        self.geometry("760x560")

        self._build_ui()
        self._refresh_map_list()

    def _build_ui(self):
        main = ttk.Frame(self)
        main.pack(fill="both", expand=True, padx=10, pady=10)

        warn = ttk.Label(
            main,
            text="Close porymap (or any other map editor) before renaming - "
                 "two tools saving the same files at once will corrupt your changes.",
            foreground="#b00020", wraplength=720, justify="left",
        )
        warn.pack(fill="x", pady=(0, 8))

        pick_frame = ttk.LabelFrame(main, text="1. Map to rename")
        pick_frame.pack(fill="x")

        row = ttk.Frame(pick_frame)
        row.pack(fill="x", padx=6, pady=6)
        ttk.Label(row, text="Old name:").pack(side="left")
        self.old_name_var = tk.StringVar()
        self.old_combo = ttk.Combobox(row, textvariable=self.old_name_var, width=40, state="readonly")
        self.old_combo.pack(side="left", padx=(4, 8))
        ttk.Button(row, text="Refresh List", command=self._refresh_map_list).pack(side="left")

        row2 = ttk.Frame(pick_frame)
        row2.pack(fill="x", padx=6, pady=(0, 6))
        ttk.Label(row2, text="New name:").pack(side="left")
        self.new_name_var = tk.StringVar()
        ttk.Entry(row2, textvariable=self.new_name_var, width=40).pack(side="left", padx=(4, 8))

        opts_frame = ttk.LabelFrame(main, text="2. Options")
        opts_frame.pack(fill="x", pady=(8, 0))
        self.rename_layout_var = tk.BooleanVar(value=True)
        ttk.Checkbutton(
            opts_frame,
            text="Also rename the layout (only if it isn't shared with another map)",
            variable=self.rename_layout_var,
        ).pack(anchor="w", padx=6, pady=6)

        btn_frame = ttk.Frame(main)
        btn_frame.pack(fill="x", pady=8)
        self.dry_run_btn = ttk.Button(btn_frame, text="Dry Run (preview only)", command=self._dry_run)
        self.dry_run_btn.pack(side="left")
        self.apply_btn = ttk.Button(btn_frame, text="Rename", command=self._apply)
        self.apply_btn.pack(side="left", padx=8)

        log_frame = ttk.LabelFrame(main, text="Output")
        log_frame.pack(fill="both", expand=True)
        self.log = tk.Text(log_frame, state="disabled", wrap="word", height=18)
        scrollbar = ttk.Scrollbar(log_frame, orient="vertical", command=self.log.yview)
        self.log.configure(yscrollcommand=scrollbar.set)
        self.log.pack(side="left", fill="both", expand=True, padx=(6, 0), pady=6)
        scrollbar.pack(side="right", fill="y", pady=6)

    def _refresh_map_list(self):
        names = sorted(p.name for p in MAPS_DIR.iterdir() if p.is_dir() and (p / "map.json").is_file())
        self.old_combo["values"] = names
        if names and self.old_name_var.get() not in names:
            self.old_name_var.set(names[0])

    def _log(self, msg):
        self.log.configure(state="normal")
        self.log.insert("end", msg)
        self.log.see("end")
        self.log.configure(state="disabled")

    def _set_running(self, running):
        state = "disabled" if running else "normal"
        self.dry_run_btn.configure(state=state)
        self.apply_btn.configure(state=state)

    def _validated_args(self):
        old_name = self.old_name_var.get().strip()
        new_name = self.new_name_var.get().strip()
        if not old_name:
            messagebox.showerror("Missing input", "Pick a map to rename.")
            return None
        if not new_name:
            messagebox.showerror("Missing input", "Type the new map name.")
            return None
        if new_name == old_name:
            messagebox.showerror("Invalid input", "New name is the same as the old name.")
            return None
        return old_name, new_name

    def _run(self, extra_args):
        args = self._validated_args()
        if args is None:
            return
        old_name, new_name = args
        cmd = [sys.executable, str(RENAME_SCRIPT), old_name, new_name, *extra_args]

        self.log.configure(state="normal")
        self.log.delete("1.0", "end")
        self.log.configure(state="disabled")
        self._log("$ " + " ".join(cmd) + "\n\n")
        self._set_running(True)

        def worker():
            try:
                proc = subprocess.Popen(
                    cmd, cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, bufsize=1,
                )
                for line in proc.stdout:
                    self.after(0, self._log, line)
                proc.wait()
                if proc.returncode != 0:
                    self.after(0, self._log, f"\n(exited with code {proc.returncode})\n")
            except Exception as exc:
                self.after(0, self._log, f"\nerror: {exc}\n")
            finally:
                self.after(0, self._set_running, False)
                self.after(0, self._refresh_map_list)

        threading.Thread(target=worker, daemon=True).start()

    def _dry_run(self):
        self._run(["--dry-run"] + ([] if self.rename_layout_var.get() else ["--no-layout-rename"]))

    def _apply(self):
        args = self._validated_args()
        if args is None:
            return
        old_name, new_name = args
        if not messagebox.askyesno(
            "Confirm rename",
            f"Rename map {old_name!r} to {new_name!r}?\n\n"
            "This edits files in your working tree (via git mv + text edits). "
            "Make sure your map editor is closed first.",
        ):
            return
        self._run([] if self.rename_layout_var.get() else ["--no-layout-rename"])


if __name__ == "__main__":
    RenameMapApp().mainloop()
