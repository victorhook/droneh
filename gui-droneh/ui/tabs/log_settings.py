import tkinter as tk
from tkinter import simpledialog, messagebox
from tkinter import ttk
import typing as t
import struct

from client import log
from client import protocol
from utils.settings import Settings
from ui.parameters import ParamWindow
from ui.checkbox_values import CheckBoxValues
from ui.tabs.tab import Tab


class LogSettings(Tab):

    def __init__(self, master):
        super().__init__(master)

        self.log_configs = {conf.name: conf for conf in log.get_log_configs()}
        self.log_params = {param.name: param for param in log.get_available_log_params()}

        self.active_tree = self.ActiveLogTree(self)
        self.active_sidebar = self.ActiveLogSidebar(self)

        if self.log_configs:
            active_config = list(self.log_configs.values())[0]
            self.active_config_set(active_config)

        pads = {'padx':10, 'pady':10}
        self.active_sidebar.pack(side=tk.LEFT, fill=tk.Y, **pads)
        self.active_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, **pads)

    def switch_to(self) -> None:
        self.log_configs = {conf.name: conf for conf in log.get_log_configs()}
        self.log_params = {param.name: param for param in log.get_available_log_params()}
        if self.log_configs:
            active_config = list(self.log_configs.values())[0]
            self.active_config_set(active_config)

    def _valid_log_config_name(self, name: str) -> bool:
        return True

    def active_config_new(self):
        """ Adds a new log configuration and sets it as active. """
        name = simpledialog.askstring('New log configuration', 'Name: ')
        if self._valid_log_config_name(name):
            new_config = log.LogConfig(name, [], 50)
            self.log_configs[name] = new_config
            self.active_config_set(new_config)
            self.active_config_save()

    def active_config_edit(self):
        """ Edits the active configuration name. """
        if self.active_config is None:
            return
        old_name = self.active_config.name
        new_name = simpledialog.askstring('Edit log configuration', f'Change name from "{old_name}"')
        if self._valid_log_config_name(new_name):
            self.active_config.name = new_name
            self.active_config_set(self.active_config)
            self.active_config_save()

    def active_config_delete(self):
        """ Deletes the active configuration. """
        if self.active_config.name is None:
            return
        name = self.active_config.name
        yes = messagebox.askyesno('Delete confguration', f'Do you want to delete log config "{name}"')
        if yes:
            self.log_configs.pop(name)
            if self.log_configs:
                active_config = list(self.log_configs.values())[0]
            else:
                active_config = None
            self.active_config_set(active_config)
            self.active_config_save()

    def active_config_save(self):
        """ Deletes the active configuration. """
        self.active_config.frequency_ms = self.active_tree.get_frequency()
        log.save_log_configs(list(self.log_configs.values()))
        self.active_tree.set_status('Saved')

    def active_config_set(self, config: log.LogConfig):
        """ Sets the active configuration to the new one. """
        self.active_config = config
        if self.active_config is None:
            return
        self.update_sidebar()
        self.update_tree()

    def update_sidebar(self):
        self.active_sidebar.update_dropdown(self.active_config, list(self.log_configs.values()))
        self.active_sidebar.update_checkboxes(self.active_config, list(self.log_params.values()))

    def update_tree(self):
        self.active_tree.update_tree(self.active_config)
        self.active_tree.update_total_bytes(self.active_config)
        self.active_tree.update_frequency(self.active_config)

    def parameter_change(self, params: t.Dict[str, str]):
        """ Updates the active configuration to match the changed params. """
        self.active_config.params = self._get_selected_params(params)
        self.update_tree()

    def _get_selected_params(self, params: t.Dict[str, str]):
        selected = []
        for param, checked in params.items():
            if checked:
                selected.append(self.log_params[param])
        return selected


    class ActiveLogTree(ttk.Frame):

        def __init__(self, master):
            super().__init__(master, style='TFrame')
            self.title = ttk.Label(self, style='TLabel', text='sss')

            self.frame_top = ttk.Frame(self, style='TFrame')
            self.freq_label = ttk.Label(self.frame_top, text='Log Frequency (ms): ', style='TLabel')
            self.freq_input = ttk.Entry(self.frame_top, width=5, style='TEntry')
            self.btn_save = ttk.Button(self.frame_top, text='Save', command=master.active_config_save, style='TButton')
            self.btn_new = ttk.Button(self.frame_top, text='New', command=master.active_config_new, style='TButton')
            self.btn_del = ttk.Button(self.frame_top, text='Delete', command=master.active_config_delete, style='TButton')
            self.btn_edit = ttk.Button(self.frame_top, text='Edit', command=master.active_config_edit, style='TButton')
            self.status = ttk.Label(self.frame_top, text='', style='TLabel')
            pad = {'padx': 5, 'pady': 5}
            self.freq_label.grid(row=0, column=0, **pad)
            self.freq_input.grid(row=0, column=1, **pad)
            self.btn_save.grid(row=0, column=2, **pad)
            self.btn_new.grid(row=0, column=3, **pad)
            self.btn_del.grid(row=0, column=4, **pad)
            self.btn_edit.grid(row=0, column=5, **pad)
            self.status.grid(row=0, column=6, **pad)

            self.tree = ttk.Treeview(self, columns=['name', 'type', 'size'])
            self.tree['show'] = 'headings'
            self.tree.heading('name', text='Name')
            self.tree.heading('type', text='Type')
            self.tree.heading('size', text='Size')

            self.bytes = ttk.Label(self, style='TLabel')
            self.progress = ttk.Progressbar(self, length=400, style='TProgressbar')

            self.title.pack(side=tk.TOP, pady=5)
            self.frame_top.pack()
            self.tree.pack(side=tk.TOP, fill=tk.BOTH, expand=True)
            self.bytes.pack(side=tk.TOP, pady=5)
            self.progress.pack(side=tk.BOTTOM)

        def set_status(self, msg: str) -> None:
            self.status.config(text=msg)
            self.after(2000, lambda: self.status.config(text=''))

        def get_frequency(self) -> int:
            try:
                return int(self.freq_input.get())
            except:
                return 0

        def update_tree(self, active_config: log.LogConfig):
            self.title.config(text=active_config.name)

            for child in self.tree.get_children():
                self.tree.delete(child)
            for param in active_config.params:
                self.tree.insert('', tk.END, values=[
                    param.name,
                    param.log_type,
                    struct.calcsize(param.log_type)
                ])

        def update_total_bytes(self, active_config: log.LogConfig):
            byte_size = log.get_log_config_byte_size(active_config)
            filled = int((byte_size / protocol.PACKET_PAYLOAD_SIZE) * 100)
            self.progress['value'] = filled
            self.bytes.config(text = f'Bytes: {byte_size}/{protocol.PACKET_PAYLOAD_SIZE}')

        def update_frequency(self, active_config: log.LogConfig):
            self.freq_input.delete(0, tk.END)
            self.freq_input.insert(0, str(active_config.frequency_ms))



    class ActiveLogSidebar(ttk.Frame):
        def __init__(self, master):
            super().__init__(master, style='TFrame')
            self.master = master
            self.dropdown = ttk.Combobox(self, values=[], state='readonly')
            self.checkboxes = CheckBoxValues(self, on_change=master.parameter_change)
            self.dropdown.bind("<<ComboboxSelected>>", self._on_selection)

            self.dropdown.pack(side=tk.TOP, pady=10)
            self.checkboxes.pack(side=tk.TOP, fill=tk.Y, expand=True)


        def _on_selection(self, ignore=None):
            new_active_config = self.master.log_configs[self.dropdown.get()]
            self.master.active_config_set(new_active_config)

        def update_checkboxes(self, active_config: log.LogConfig, all_params: t.List[log.LogParam]):
            # Convert LogParam to dict: {str: bool} that checkboxes handles.
            checkbox_params = {param.name: False for param in all_params}
            for param in active_config.params:
                # All param in active config will be checked, all other unchecked.
                checkbox_params[param.name] = True
            self.checkboxes.set_params(checkbox_params)

        def update_dropdown(self, active_config: log.LogConfig, all_configs: t.List[log.LogConfig]):
            # Set dropdown values to the names of the configs.
            self.dropdown.config(values=[conf.name for conf in all_configs])
            index = list(self.dropdown['values']).index(active_config.name)
            self.dropdown.current(index)


    @classmethod
    def get_all(cls) -> t.List[str]:
        return []
        pass

    def _update(self):
        log.get_log_configs()
        #self._log_groups = [LogConfig(**group) for group in Settings.get('log_groups')]

