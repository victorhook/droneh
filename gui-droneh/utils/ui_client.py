import typing as t
import threading
from client import log
from client.backends import Backend
from client.client import Client


class Droneh:
    """ Singleton """
    ___client = None


    @classmethod
    def init(cls, backend: Backend) -> None:
        cls.__client = Client(backend)

    @classmethod
    def add_cb_on_connect(cls, cb: callable) -> None:
        cls.__client.cb_on_connect.add_callback(cb)

    @classmethod
    def add_cb_on_disconnect(cls, cb: callable) -> None:
        cls.__client.cb_on_disconnect.add_callback(cb)

    @classmethod
    def add_cb_on_error(cls, cb: callable) -> None:
        cls.__client.cb_on_error.add_callback(cb)

    @classmethod
    def add_cb_on_arm_change(cls, cb: callable) -> None:
        cls.__client.cb_on_arm_change.add_callback(cb)

    @classmethod
    def set_motor_thrust(cls, m1: int, m2: int, m3: int, m4: int) -> None:
        cls.__client.set_motor_thrust(m1, m2, m3, m4)

    @classmethod
    def arm(cls):
        cls.__client.arm()

    @classmethod
    def disarm(cls):
        cls.__client.disarm()

    @classmethod
    def add_log_config(cls, log_config: log.LogConfig, on_new_data: callable) -> None:
        log_config = log.ActiveLogConfig(
            name=log_config.name,
            params=log_config.params,
            frequency_ms=log_config.frequency_ms,
            on_new_data=on_new_data
        )
        cls.__client.add_log_config(log_config)

    @classmethod
    def delete_log_config(cls, log_config_name: str) -> None:
        cls.__client.delete_log_config(log_config_name)

    @classmethod
    def connect(cls) -> None:
        threading.Thread(target=cls._run, daemon=True).start()

    @classmethod
    def disconnect(cls) -> None:
        cls.__client.disconnect()

    @classmethod
    def _run(cls):
        cls.__client.connect()
        cls.__client.start()

    @classmethod
    def is_connected(cls) -> bool:
        return cls.__client.is_connected()


    @classmethod
    def get_available_log_parameters(cls) -> t.List[log.LogParam]:
        pass



if __name__ == '__main__':
    Droneh(1)