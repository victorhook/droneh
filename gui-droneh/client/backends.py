import serial
from serial.serialutil import SerialException
from abc import ABC, abstractmethod


class Backend(ABC):

    def __init__(self, timeout_ms: int) -> None:
        self._timeout_ms = timeout_ms

    def set_timeout_ms(self, timeout_ms: int) -> None:
        self._timeout_ms = timeout_ms

    @abstractmethod
    def open(self) -> bool:
        return True

    @abstractmethod
    def close(self) -> bool:
        return True

    @abstractmethod
    def read(self, length: int) -> bytes:
        pass

    @abstractmethod
    def write(self, data: bytes) -> None:
        pass


class DummyBackend(Backend):

    def __init__(self) -> None:
        super().__init__(100)

    def open(self) -> bool:
        pass

    def close(self) -> bool:
        pass

    def read(self, length: int) -> bytes:
        return bytearray(32)

    def write(self, data: bytes) -> None:
        pass


class SerialBackend(Backend):

    def __init__(self, baudrate: int, port: str, timeout_ms: int) -> None:
        super().__init__(timeout_ms)
        self._baudrate = baudrate
        self._port = port
        self.open()

    def open(self) -> bool:
        try:
            self._serial = serial.Serial(baudrate=self._baudrate, port=self._port, timeout=self._timeout_ms)
            self._serial.flush()
        except SerialException as e:
            print(f'Failed to connect to device on serial port {self._port}')
            self._serial = DummyBackend()
        return True

    def close(self) -> bool:
        self._serial.flush()
        self._serial.close()
        return True

    def read(self, length: int) -> bytes:
        return self._serial.read(length)

    def write(self, data: bytes) -> None:
        self._serial.write(data)


if __name__ == '__main__':
    s = serial.Serial(baudrate=9600, port='/dev/ttyACM0')
    print(s.read(5))