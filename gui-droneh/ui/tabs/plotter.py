import tkinter as tk
from tkinter import Frame, ttk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import numpy as np
import client
import typing as t
import threading

from client import log
from client.log import LogConfig, LogParam
from ui.tabs.log_settings import LogSettings
from ui.parameters import ParamWindow
from ui.checkbox_values import CheckBoxValues
from utils.settings import Settings
from utils.ui_client import Droneh
from ui.tabs.tab import Tab



class Plotter(Tab):

    def __init__(self, master):
        super().__init__(master)
        self.colors = ['red', 'blue', 'purple', 'yellow', 'green', 'black', 'cyan']

        # -- Canvas -- #
        self._fig = Figure(figsize=(14, 8))
        self._plot = self._fig.add_subplot(111)
        self.canvas = FigureCanvasTkAgg(self._fig, master=self)
        self.canvas.draw()

        # -- Sidebar -- #
        self.sidebar_frame = ttk.Frame(self, style='TFrame')
        self.sidebar = self.LogSidebar(self, self.sidebar_frame)
        self.controlbox = ttk.Frame(self.sidebar_frame, style='TFrame')
        self.sidebar.pack(side=tk.TOP)
        self.controlbox.pack(side=tk.BOTTOM)

        # -- Buttons for control -- #
        self.btn_start = ttk.Button(self.controlbox, text='Start', style='TButton', command=self._start)
        self.btn_stop = ttk.Button(self.controlbox, text='Stop', style='TButton', command=self._stop)
        self.btn_start.pack(side=tk.LEFT)
        self.btn_stop.pack(side=tk.LEFT)

        self.sidebar_frame.pack(side=tk.LEFT, fill=tk.Y, padx=20, pady=20)
        self.canvas.get_tk_widget().pack(side=tk.LEFT, expand=True, fill=tk.BOTH)

        # -- Plotting variables -- #
        self.MAX_VALUES = 200
        self.values: t.Dict[str, np.ndarray] = {}
        self.active_plot_params: t.List[LogParam] = []
        self.update_values_lock = threading.Lock()

        self._running = False

        self.switch_to()

    def switch_to(self) -> None:
        self.log_configs = {conf.name: conf for conf in log.get_log_configs()}
        if self.log_configs:
            active_config = list(self.log_configs.values())[0]
        else:
            active_config = None
        self.active_config_set(active_config)
        self.sidebar.on_param_select_change()    # Update params for 1st time

    def _get_param_by_name(self, name: str) -> LogParam:
        if self.active_config is None:
            return
        for param in self.active_config.params:
            if param.name == name:
                return param

    def active_config_set(self, active_config: LogConfig):
        if type(active_config) is str:
            active_config = self.log_configs[active_config]

        self.active_config = active_config
        if self.active_config is None:
            return

        # Update dropdown
        self.sidebar.update_dropdown(list(self.log_configs.keys()), self.active_config)
        params = {p.name: p for p in self.active_config.params}
        self.active_params_set(params)

    def active_params_set(self, active_params: t.Dict[str, bool]):
        if self.active_config is None:
            return

        # Set active parameters in list.
        self.active_plot_params.clear()
        for name, is_active in active_params.items():
            if is_active:
                self.active_plot_params.append(self._get_param_by_name(name))

        # Set logging variables for plotting.
        self.update_values_lock.acquire()
        self.values.clear()
        for param in self.active_plot_params:
            self.values[param.name] = np.zeros(self.MAX_VALUES)
        self.update_values_lock.release()

    def _start(self):
        if self._running or self.active_config is None:
            return

        self._running = True

        Droneh.add_log_config(self.active_config, self._on_new_data)

    def k(self, *arg):
        print('yee', arg)

    def _stop(self):
        self._running = False
        Droneh.delete_log_config(self.active_config.name)

    def _shift_array(self, vec, new_value):
        vec = np.roll(vec, -1)
        vec[len(vec)-1] = new_value
        return vec

    def _on_new_data(self, params: t.Dict[str, LogParam]):
        self._plot.cla()

        # New data that we've received
        new_data = {name: p.value for name, p in params.items()}

        # Iterate over the plotted values and update the arrays and plots
        # with the new data.
        new_values = {}
        self.update_values_lock.acquire()

        try:
            for i, param_name in enumerate(self.values):
                new_value = new_data[param_name]
                values = self._shift_array(self.values[param_name], new_value)
                self._plot.plot(range(len(values)), values, color=self.colors[i], label=param_name)
                new_values[param_name] = values
        except KeyError as e:
            print('keyerror', e)
            self.update_values_lock.release()
            return

        # Update values to newly plotted ones. (Can't be done during iteration)
        self.values = new_values
        self.update_values_lock.release()

        # Update labels
        for param_name in filter(lambda p: p in self.values, params):
            self.sidebar.set_param_value(params[param_name])

        self._plot.legend(bbox_to_anchor=(1,1), loc="upper right",)
        self.canvas.draw()

    class LogSidebar(ttk.Labelframe):

        def __init__(self, parent, master):
            self.parent = parent
            super().__init__(master, text='Active log', style='TFrame')

            self.dropdown = ttk.Combobox(self, values=[], style='TCombobox', state='readonly')
            self.param_frame = ttk.Frame(self, style='TFrame')
            self.params: t.Dict[str, self.Param] = {}

            self.dropdown.bind('<<ComboboxSelected>>', self._dropdown_change)

            self.dropdown.pack(side=tk.TOP, pady=10)
            self.param_frame.pack(side=tk.TOP)

        def set_param_value(self, param: LogParam):
            if param.log_type == 'f':
                value = round(param.value, 4)
            else:
                value = param.value
            self.params[param.name].set_value(str(value))

        def on_param_select_change(self):
            selected_params: t.Dict[str, bool] = {}
            for param in self.params.values():
                selected_params[param.name] = param.is_selected()

            self.parent.active_params_set(selected_params)

        def update_dropdown(self, available_log_configs: t.List[str], active_config: LogConfig):
            self.dropdown['values'] = available_log_configs
            if active_config is None:
                return

            index = self.dropdown['values'].index(active_config.name)
            self.dropdown.current(index)
            self.build_params(active_config.params)

        def build_params(self, params: t.List[LogParam]):
            for child in self.param_frame.winfo_children():
                child.destroy()
            self.params.clear()

            for row, param in enumerate(params):
                param_ui = self.Param(self.param_frame, param.name, row, self.on_param_select_change)
                self.params[param.name] = param_ui

        def _dropdown_change(self, ignore):
            active_config = list(self.dropdown['values'])[self.dropdown.current()]
            self.parent.active_config_set(active_config)

        # -- Nested class -- #
        class Param(ttk.Frame):
            def __init__(self, master, label, row, on_change, value=0):
                self.name = label
                self.var = tk.IntVar()
                self.var.set(1)
                self.check = ttk.Checkbutton(master, style='TCheckbutton', variable=self.var, command=on_change)
                self.label = ttk.Label(master, text=label, style='TLabel')
                self.value = ttk.Label(master, text=str(value), style='TLabel', width=8)
                self.check.grid(row=row, column=0, sticky=tk.W)
                self.label.grid(row=row, column=1, sticky=tk.W)
                self.value.grid(row=row, column=2, sticky=tk.W)
                self.on_change = on_change

            def set_value(self, value):
                self.value.config(text=str(value))

            def is_selected(self):
                return self.var.get() == 1