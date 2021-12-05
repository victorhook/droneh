from dataclasses import dataclass, asdict
import typing as t
from pathlib import Path
import json
import struct

LOG_PARAM_PATH = Path(__file__).parent.joinpath('log_params.json')
LOG_GROUP_PATH = Path(__file__).parent.joinpath('log_configs.json')
LOG_PARAM_MAX_LENGTH = 10


class LogTypes:
    U8 = 0
    I8 = 1
    U16 = 2
    I16 = 3
    U32 = 4
    I32 = 5
    F32 = 6
    F64 = 7

    _STRINGS = {
        0: 'U8',
        1: 'I8',
        2: 'U16',
        3: 'I16',
        4: 'U32',
        5: 'I32',
        6: 'F32',
        7: 'F64',
    }


@dataclass
class LogParam:
    id: int
    log_type: int
    name: str
    value: type = None

    def __eq__(self, other):
        if type(other) is type(self):
            return self.name == other.name
        elif type(other) is str:
            return self.name == other
        return False


@dataclass
class LogConfig:
    name: str
    params: t.List[LogParam]
    frequency_ms: int

    def as_dict(self):
        return asdict(self)


@dataclass
class ActiveLogConfig(LogConfig):
    on_new_data: callable
    last_sent: int = 0

    def __iter__(self):
        return self.params


class Logger:

    def __init__(self) -> None:
        self._configs: t.List[LogConfig] = []

    def get_log_configs(self) -> t.List[LogConfig]:
        return self._config

    def add_log_config(self, log_config: LogConfig) -> None:
        self._configs.append(log_config)


def _save(items: list, path: str) -> None:
    if not items:
        return
    with open(path, 'w') as f:
        json.dump([asdict(item) for item in items], f, indent=4)

def _get(path: str, cls: type) -> list:
    with open(path) as f:
        items = json.load(f)
        items = [cls(**item) for item in items]
        return items

def save_available_log_params(log_params: t.List[LogParam]):
    _save(log_params, LOG_PARAM_PATH)

def get_available_log_params() -> t.List[LogParam]:
    return _get(LOG_PARAM_PATH, LogParam)

def save_log_configs(log_configs: t.List[LogConfig]):
    _save(log_configs, LOG_GROUP_PATH)

def get_log_configs() -> t.List[LogParam]:
    configs = _get(LOG_GROUP_PATH, LogConfig)
    for config in configs:
        config.params = [LogParam(**param) for param in config.params]

    all_params = {p.name: p for p in get_available_log_params()}
    for config in configs:
        delete = []
        for param in config.params:
            try:
                # Ensure that id of parameter is correct.
                param.id = all_params[param.name].id
            except KeyError:
                # If we can't find param from config in available parameters,
                # it must be an old parameter and we must remove it.
                delete.append(param)
        # Remove all old params.
        for param in delete:
            config.params.remove(param)


    return configs

def get_log_config_byte_size(config: LogConfig) -> int:
    return sum(struct.calcsize(param.log_type) for param in config.params)


if __name__ == '__main__':
    print(get_log_configs())