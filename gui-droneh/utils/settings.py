import json
from pathlib import Path


class Settings:
    _settings: dict = None
    _base_path = Path(__file__).parent.joinpath('settings.json')

    @classmethod
    def _save(cls):
        with open(cls._base_path, 'w') as f:
            json.dump(cls._settings, f, indent=4)

    @classmethod
    def set_key_value_pair(cls, key, value):
        settings = cls.get()
        settings[key] = value
        cls._save()

    @classmethod
    def set(cls, settings: dict):
        cls._settings = settings
        cls._save()

    @classmethod
    def get(cls, key: str = None) -> dict:
        if cls._settings is None:
            try:
                with open(str(cls._base_path)) as f:
                    cls._settings = json.load(f)
            except FileNotFoundError:
                cls._settings = {}
                cls._save()

        return cls._settings.get(key, None) if key is not None else cls._settings
