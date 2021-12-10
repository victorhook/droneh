import tkinter as tk
from tkinter import ttk
from tkinter.constants import LEFT

from ui.tabs.fly import Fly
from ui.tabs.motor_test import MotorTest
from ui.tabs.plotter import Plotter
from ui.tabs.log_settings import LogSettings

from ui.parameters import ParamWindow


class Center(ttk.Frame):

    def __init__(self, master):
        super().__init__(master, style='TFrame')

        self.params = ParamWindow(self)
        self.tabs = ttk.Notebook(self, style='TNotebook')

        self.tabs.add(MotorTest(self.tabs), text='Motor Test')
        self.tabs.add(Plotter(self.tabs), text='Plotter')
        self.tabs.add(LogSettings(self.tabs), text='Log Settings')
        self.tabs.add(Fly(self.tabs), text='Fly')
        self._selected_tab = None
        self.tabs.select(3)

        self.tabs.bind("<<NotebookTabChanged>>", self._select_tab)

        self.params.pack(side=tk.LEFT, padx=5, fill=tk.Y)
        self.tabs.pack(side=tk.LEFT, padx=5, fill=tk.BOTH, expand=True)

    def _select_tab(self, ignore):
        if self._selected_tab is not None:
            self._selected_tab.switch_from()

        tab_name = self.tabs.select()
        tab = self.tabs.nametowidget(tab_name)
        tab.switch_to()
        self._selected_tab = tab
