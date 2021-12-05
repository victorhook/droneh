import tkinter as tk
from tkinter import font, ttk

BG = 'white'
FONT = 'Courier'
COLOR_PRIMARY = 'green'
COLOR_SECONDARY = 'white'

style = ttk.Style()
style.configure(
    "TLabel",
    foreground="black",
    background="white",
    font=(FONT, 18),
    padding=10
)

style.configure(
    "TFrame",
    background="white",
    highlightcolor= 'black',
    highlightthickness=5
)

style.configure(
    "TCheckbutton",
    background="white",
    font=(FONT, 18),
)

style.configure(
    "TLabelframe",
    background="white"
)

style.configure(
    "TButton",
    background=COLOR_PRIMARY,
    foreground=COLOR_SECONDARY
)

style.configure(
    "TNotebook",
    background="white",
    font=(FONT, 18)
)
style.configure(
    "TNotebook.Tab",
    background="white",
    foreground='red',
    lightcolor='blue',
    font=(FONT, 18),
)
style.configure(
    "TScale",
    background="blue",
)