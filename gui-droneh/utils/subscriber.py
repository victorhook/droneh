from utils.state import UiState
from abc import abstractclassmethod


class Subscriber:

    def __init__(self) -> None:
        UiState.subscribe(UiState.CONNECTED, self._connect)
        UiState.subscribe(UiState.DISCONNECTED, self._disconnect)

    @abstractclassmethod
    def _connect(self, state: str) -> None:
        print('yep')
        pass

    @abstractclassmethod
    def _disconnect(self, state: str) -> None:
        pass
