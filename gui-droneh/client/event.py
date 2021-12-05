from dataclasses import dataclass, field
import queue
import typing as t
from queue import Queue
import threading


class EventType:
    WRITE = 'write'
    READ  = 'read'


@dataclass
class Event:
    type: EventType
    thread_name: str
    payload: str = None
    call: callable = None


class EventQueue:

    def __init__(self) -> None:
        self._thread_name = threading.current_thread().getName()
        self._queue = Queue()

    def put(self, event: Event) -> None:
        self._queue.put(event)

    def execute(self, event: Event):
        pass


q = Queue()
q.put(Event(EventType.WRITE, '1', lambda msg: print(msg)))
q.put(Event(EventType.WRITE, '2', lambda msg: print(msg)))
q.put(Event(EventType.WRITE, '3', lambda msg: print(msg)))


def ex(msg):
    q.put(
        Event(
            EventType.WRITE,
            threading.current_thread().getName()
            msg,
            lambda msg: print(msg)
        )
    )
    task = q.get()
    task.call(task.payload)


def transmit(msg):
    ex(msg)


transmit('hey')
print(threading.current_thread().getName())