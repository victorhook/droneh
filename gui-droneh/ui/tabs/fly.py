import enum
import tkinter as tk
from tkinter import ttk
from dataclasses import dataclass
from typing import Set
import numpy as np

from ui.tabs.tab import Tab
from utils.ui_client import Droneh


@dataclass
class Setpoint:
    roll: int
    pitch: int
    yaw: int
    thrust: int
    MAX = 1023
    MIN = 0

class Fly(Tab):

    def __init__(self, master):
        super().__init__(master)

        self.info_frame = ttk.Frame(self)
        self.setpoints = self.SetPoints(self.info_frame)
        self.logging = self.Logging(self.info_frame)
        self.canvas = self.Canvas(self)

        self.setpoints.pack(side=tk.LEFT)
        self.logging.pack(side=tk.LEFT)

        self.info_frame.pack(side=tk.TOP, fill=tk.BOTH, expand=True)
        self.canvas.pack(side=tk.BOTTOM, fill=tk.X, expand=True)


    class Canvas(tk.Canvas):
        def __init__(self, master):
            super().__init__(master)
            self.update_idletasks()
            self.r = 50
            self.left = self.Joystick(self, self.r,  300, 150)
            self.right = self.Joystick(self, self.r, 850, 150)

            self.i = 512
            self.set(Setpoint(512, 512, 512, 512))
            self.k()

        class Joystick:
            def __init__(self, canvas: tk.Canvas, radius: int, x: int, y: int):
                self.r = radius
                self.c = canvas
                self.x = x
                self.y = y
                x0, y0 = x - self.r, y - self.r
                canvas.create_line(self.x, self.y-2*self.r, self.x, self.y+2*self.r, fill='black')
                canvas.create_line(self.x-2*self.r, self.y, self.x+2*self.r, self.y, fill='black')
                self.id = canvas.create_oval(x0-self.r, y0-self.r, x0+self.r, y0+self.r, fill='red')

            def set(self, cx: int, cy: int):
                dx = cx - 512
                dy = cy - 512

                dx /= 512
                dy /= 512

                dx *= self.r
                dy *= self.r

                x0 = self.x + dx
                y0 = self.y + dy

                self.c.coords(self.id, x0-self.r, y0-self.r, x0+self.r, y0+self.r)

        def set(self, setpoint: Setpoint) -> None:
            self.left.set(setpoint.yaw, setpoint.thrust)
            self.right.set(setpoint.roll, setpoint.pitch)

        def k(self):
            self.set(Setpoint(self.i, self.i, self.i, self.i))
            self.i += 10
            if (self.i > 1023):
                self.i = 0
            self.after(50, self.k)


    class SetPoints(ttk.Frame):
        def __init__(self, master):
            super().__init__(master, style='TFrame')
            style = {'sticky': tk.W}
            ttk.Label(self, text='Setpoints', style='TLabel').grid(row=0, columnspan=2)
            ttk.Label(self, text='Roll: ', style='TLabel').grid(row=1, column=0, **style)
            ttk.Label(self, text='Pitch: ', style='TLabel').grid(row=2, column=0, **style)
            ttk.Label(self, text='Yaw: ', style='TLabel').grid(row=3, column=0, **style)
            ttk.Label(self, text='Thrust: ', style='TLabel').grid(row=4, column=0, **style)
            self._roll = ttk.Label(self, style='TLabel').grid(row=1, column=1, **style)
            self._pitch = ttk.Label(self, style='TLabel').grid(row=2, column=1, **style)
            self._yaw = ttk.Label(self, style='TLabel').grid(row=3, column=1, **style)
            self._thrust = ttk.Label(self, style='TLabel').grid(row=4, column=1, **style)

    class Logging(ttk.Frame):
        def __init__(self, master):
            super().__init__(master, style='TFrame')