import tkinter as tk
from tkinter import ttk
import typing as t
from dataclasses import dataclass
from tkscrolledframe import ScrolledFrame


class CheckBoxValues(ttk.Frame):

    def __init__(self, master, params: t.Dict[str, bool] = {}, on_change: callable = None):
        super().__init__(master)

        self.frame = ScrolledFrame(self, use_ttk=True, scrollbars='vertical')
        self.frame.pack(fill=tk.Y, expand=True)
        self.inner_frame = self.frame.display_widget(ttk.Frame, style='TFrame')

        self._params = params
        self._params_ui = {}
        self._on_change = on_change
        self._init = False
        if params is not None:
            self._update_param_ui()


    def _on_checkbutton_change(self):
        if self._init:
            for name, checkbox in self._params_ui.items():
                checked = 'selected' in checkbox.state()
                self._params[name] = checked
            if self._on_change:
                self._on_change(self._params)

    def set_params(self, params: t.Dict[str, bool]):
        self._clear()
        self._params = params
        self._update_param_ui()

    def _clear(self):
        for child in self.inner_frame.winfo_children():
            child.destroy()
        self._params_ui = {}
        self._init = False

    def _update_param_ui(self):
        row = 0
        for name, checked in self._params.items():
            box = ttk.Checkbutton(self.inner_frame, text=name, style='TCheckbutton',
                                  command=self._on_checkbutton_change)
            box.grid(row=row, sticky=tk.W)
            row += 1
            box.invoke()
            if not checked:
                box.invoke()
            self._params_ui[name] = box

        self._init = True

