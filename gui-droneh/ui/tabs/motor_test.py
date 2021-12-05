import enum
import tkinter as tk
from tkinter import ttk

from ui.tabs.tab import Tab
from utils.ui_client import Droneh


class MotorTest(Tab):

    motors = {'M1': 0, 'M2': 0, 'M3': 0, 'M4': 0}

    def __init__(self, master):
        super().__init__(master)
        self.motor_frame = ttk.Frame(self, style='TFrame')
        self.control_frame = ttk.Frame(self, style='TFrame')

        # Motors
        self._motors = [self.MotorSlider(self.motor_frame, m) for m in MotorTest.motors]
        for i, m in enumerate(self._motors):
            m.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        # Buttons
        self.btn_arm = ttk.Button(self.control_frame, text='ARM', command=Droneh.arm)
        self.btn_disarm = ttk.Button(self.control_frame, text='DISARM', command=Droneh.disarm)
        self.btn_arm.pack()
        self.btn_disarm.pack()

        self.motor_frame.pack()
        self.control_frame.pack()

    def set(self, m1, m2, m3, m4):
        self.motors['M1'] = m1
        self.motors['M2'] = m2
        self.motors['M3'] = m3
        self.motors['M4'] = m4
        for motor, value in zip(self._motors, self.motors.values()):
            motor.set(value)

    class MotorSlider(ttk.Frame):

        def __init__(self, master, motor: str, min=0, max=0x00ff):
            super().__init__(master, style='TFrame')
            self._id = motor
            self._value = tk.IntVar()
            self.title = ttk.Label(self, text=motor, style='TLabel')
            self.slider = ttk.Scale(self, from_=max, to=min,
                                    command=self._update, orient='vertical',
                                    variable=self._value, style='TScale')
            self.value = ttk.Label(self, text='', style='TLabel', width=5)

            self.title.pack()
            self.slider.pack()
            self.value.pack()

            self._update(self.slider.get())

        def set(self, value: int):
            self._value.set(value)

        def _update(self, _):
            value = self._value.get()
            MotorTest.motors[self._id] = value
            self.value.config(text=str(value))
            Droneh.set_motor_thrust(
                MotorTest.motors['M1'],
                MotorTest.motors['M2'],
                MotorTest.motors['M3'],
                MotorTest.motors['M4'],
            )