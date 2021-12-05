import tkinter as tk
from tkinter import ttk
from tkinter.constants import S
from client.log import LogConfig, get_available_log_params

from utils.settings import Settings
from ui.tabs.motor_test import MotorTest
from ui.parameters import ParamWindow
from utils.state import UiState
from utils.subscriber import Subscriber
from utils.ui_client import Droneh


class BackendChooser(ttk.Labelframe):

    BACKENDS = ['Serial', 'Radio']

    def __init__(self, master):
        super().__init__(master, text='Backend', style='TLabelframe')
        self.dropdown = ttk.Combobox(self, values=self.BACKENDS, state='readonly')
        self.btn_set = ttk.Button(self, text='Set', command=self._set)

        # Set default backend
        backend = Settings.get('backend')
        if backend is None:
            backend = 'Serial'
            Settings.set_key_value_pair('backend', backend)

        self.dropdown.current(self.BACKENDS.index(backend))

        self.dropdown.pack(side=tk.LEFT)
        self.btn_set.pack(side=tk.LEFT)

    def _set(self):
        # Backend is indexed.
        Settings.set_key_value_pair('backend', self.BACKENDS[self.dropdown.current()])

class Status(ttk.Labelframe, Subscriber):

    def __init__(self, master):
        ttk.Labelframe.__init__(self, master, text='Status', style='TLabelframe')
        Subscriber.__init__(self)
        self.label = ttk.Label(self, text=UiState.get(), width=12, style='TLabel')
        self.label.pack()

    def _connect(self, state: str) -> None:
        self.label.config(text='Connected')

    def _disconnect(self, state: str) -> None:
        self.label.config(text='Disconnected')


class Armed(ttk.Frame):

    def __init__(self, master):
        super().__init__(master, style='TFrame')
        ttk.Label(self, text='Armed: ').pack(side=tk.LEFT)
        self.armed = ttk.Label(self, width=3)
        self.armed.pack(side=tk.LEFT)
        self._update(False)

        Droneh.add_cb_on_arm_change(self._update)

    def _update(self, is_armed: bool):
        print(is_armed)
        if is_armed:
            self.armed.config(text='YES', background='green')
        else:
            self.armed.config(text='NO', background='red')

class Top(ttk.Frame, Subscriber):

    def __init__(self, master):
        ttk.Frame.__init__(self, master, style='TFrame')
        Subscriber.__init__(self)

        self.status = Status(self)
        self.btn_connect = ttk.Button(self, text='Connect', style='TButton',
                                      width=12, command=self._connect_or_disconnect)
        self.backend_chooser = BackendChooser(self)
        self.armed = Armed(self)

        packing = {'side': tk.LEFT, 'padx': 5}
        self.status.pack(**packing)
        self.btn_connect.pack(**packing)
        self.backend_chooser.pack(**packing)
        self.armed.pack(side=tk.RIGHT)

    def _connect_or_disconnect(self):
        if UiState.get() == UiState.CONNECTED:
            Droneh.disconnect()
        else:
            Droneh.connect()

    def _connect(self, state: str) -> None:
        self.btn_connect.config(text='Disconnect')

    def _disconnect(self, state: str) -> None:
        self.btn_connect.config(text='Connect')