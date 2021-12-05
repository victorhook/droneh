import typing as t


class Callback:

    def __init__(self) -> None:
        self._callbacks: t.List[callable] = []

    def add_callback(self, callback: callable) -> None:
        self._callbacks.append(callback)

    def call(self, *args):
        for fn in self._callbacks:
            fn(*args)
