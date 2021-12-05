import tkinter as tk
from tkinter import ttk



class Tab(ttk.Frame):

    def __init__(self, master):
        super().__init__(master, style='TFrame')

    def switch_to(self) -> None:
        pass

    def switch_from(self) -> None:
        pass