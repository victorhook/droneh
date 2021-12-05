from dataclasses import dataclass, field
import typing as t
from collections import namedtuple


Subscription = namedtuple('Subscriber', field_names=['state', 'call'])


class UiState:
    CONNECTED = 'Connected'
    DISCONNECTED = 'Disconnected'
    __curr_state = DISCONNECTED
    __subscribers: t.Dict[str, t.List[Subscription]] = {
        CONNECTED: [],
        DISCONNECTED: []
    }

    @classmethod
    def get(cls):
        return cls.__curr_state

    @classmethod
    def set(cls, state):
        cls.__curr_state = state
        for subscriber in cls.__subscribers[state]:
            print(subscriber)
            subscriber(state)

    @classmethod
    def subscribe(cls, state: str, callback: callable):
        if type(state) is str:
            states = [state]
        for state in states:
            cls.__subscribers[state].append(callback)
