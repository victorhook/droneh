import tkinter as tk
from tkinter import ttk

from ui.center_window import Center
from ui.top import Top


class MainWindow(tk.Tk):

    def __init__(self):
        super().__init__()
        self._set_default_ui()

        self.top = Top(self)
        self.center = Center(self)

        self.top.pack(fill=tk.X, pady=(5, 50), padx=5)
        self.center.pack(fill=tk.BOTH, padx=5)

    def _set_default_ui(self):
        import ui.ui_styles as styles
        self.title('Droneh')
        self.config(bg=styles.BG)
        self.geometry(f'{1400}x{800}')


