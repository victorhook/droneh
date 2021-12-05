import tkinter as tk
from tkinter import ttk

from numpy import exp

default_params = [
    'Roll',
    'Pitch',
    'Yaw',
    'Thrust',
    'Altitude',
    'Temperature',
    'Pressure'
]

from ui.model3d import Model3D


class ParamWindow(ttk.Frame):


    def __init__(self, master, params=default_params):
        super().__init__(master)

        self.model3d = Model3D(self)

        self.params = {param: 0 for param in params}
        self.param_frame = ttk.Frame(self, style='TFrame')
        self._params = [self.Param(self.param_frame, param, 0, row) for row, param in enumerate(params)]

        self.param_frame.pack(side=tk.TOP, fill=tk.BOTH)
        self.model3d.pack(side=tk.TOP, fill=tk.BOTH, expand=True)

    class Param:

        def __init__(self, master, name: str, value: float, row: int):
            self._id = name
            self.title = ttk.Label(master, text=name, style="TLabel")
            self.value = ttk.Label(master, text=str(value), style="TLabel")
            self.title.grid(row=row, column=0, sticky=tk.W)
            self.value.grid(row=row, column=1)

        def set(self, value):
            self.value.config(text=str(value))