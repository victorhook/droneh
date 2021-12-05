import tkinter as tk
from tkinter import ttk
import typing as t
import numpy as np

import vispy
from vispy.scene import SceneCanvas
from vispy.scene.visuals import Mesh
from vispy.color import Color
from vispy.visuals.transforms import MatrixTransform
from vispy.io.stl import load_stl_binary
from client.log import LogConfig, LogParam

from utils.ui_client import Droneh
from client import log

STL_PATH = '/home/victor/coding/projects/droneh/gui-droneh/droneh.stl'

VISPY_BACKEND = "tkinter"
vispy.use(app=VISPY_BACKEND, gl="gl+")


class Model3D(ttk.Frame):

    def __init__(self, master):
        super().__init__(master, style='TFrame')
        self.canvas_frame = ttk.Frame(self, style='TFrame')

        # 3D Canvas
        self.canvas = SceneCanvas(show=True, size=(300, 300), parent=self.canvas_frame)
        self.view = self.canvas.central_widget.add_view(
            bgcolor='#fff',
            camera='turntable'
        )
        self._render_3d_model()
        self.canvas.native.pack(fill=tk.BOTH, expand=True)
        self.update_idletasks()

        # Parameters
        self.frame_labels = ttk.Frame(self, style='TFrame')
        ttk.Label(self.frame_labels, text='Roll: ').grid(row=0, column=0, sticky=tk.W)
        ttk.Label(self.frame_labels, text='Pitch: ').grid(row=1, column=0, sticky=tk.W)
        ttk.Label(self.frame_labels, text='Yaw: ').grid(row=2, column=0, sticky=tk.W)
        self._roll = ttk.Label(self.frame_labels)
        self._roll.grid(row=0, column=1, sticky=tk.W)
        self._pitch = ttk.Label(self.frame_labels)
        self._pitch.grid(row=1, column=1, sticky=tk.W)
        self._yaw = ttk.Label(self.frame_labels)
        self._yaw.grid(row=2, column=1, sticky=tk.W)

        self.roll = 0
        self.pitch = 0
        self.yaw = 0

        self.canvas_frame.pack(fill=tk.BOTH, expand=True)
        self.frame_labels.pack()

        self._first = True

        self._update_labels()
        Droneh.add_cb_on_connect(self._attach_logger)
        Droneh.add_cb_on_disconnect(self._attach_reset_attitude)

    def _render_3d_model(self, filepath: str = STL_PATH):
        with open(filepath, 'rb') as f:
            stl = load_stl_binary(f)
        self.mesh = Mesh(stl['vertices'], stl['faces'], parent=self.view.scene)
        self.mesh.transform = MatrixTransform()
        self.mesh.transform.scale((.025, .025, .025))
        self.mesh.color = Color('#ff0000')

    def _attach_logger(self, success):
        if success:
            params = {p.name: p for p in log.get_available_log_params()}
            config = LogConfig(
                'attitude',
                [
                    params['estimate_roll_deg'],
                    params['estimate_pitch_deg'],
                    params['estimate_yaw_deg']
                ],
                frequency_ms=50
            )
            Droneh.add_log_config(config, self._on_new_data)

    def _attach_reset_attitude(self, success):
        self.roll = 0
        self.pitch = 0
        self.yaw = 0

    def _on_new_data(self, params: t.Dict[str, LogParam]):
        roll = params['estimate_roll_deg'].value
        pitch = params['estimate_pitch_deg'].value
        yaw = params['estimate_yaw_deg'].value

        if self._first:
            self.roll = roll
            self.pitch = pitch
            self.yaw = yaw
            self._first = False

        droll = roll - self.roll
        dpitch = pitch - self.pitch
        dyaw = yaw - self.yaw

        try:
            self.mesh.transform.rotate(droll, np.array([0, 1, 0]))
            self.mesh.transform.rotate(-dpitch, np.array([1, 0, 0]))
        except ValueError:
            # Math issues with rotations sometimes
            pass
        #self.mesh.transform.rotate(dyaw, np.array([0, 0, 1]))

        self.roll = roll
        self.pitch = pitch
        self.yaw = yaw

        self._update_labels()

    def _update_labels(self):
        self._roll.config(text=str(int(self.roll)))
        self._pitch.config(text=str(int(self.pitch)))
        self._yaw.config(text=str(int(self.yaw)))
