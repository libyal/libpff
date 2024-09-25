"""
Type stubs for pypff_record_entries, defined in [1]

[1] https://github.com/Everlaw/libpff/blob/master/pypff/pypff_record_entries.c
"""
from typing import Iterator
from .record_entry import record_entry


class record_entries:
    ### Methods
    def __init__(self) -> None: ...

    def __iter__(self) -> Iterator[record_entry]: ...

    def __next__(self) -> record_entry: ...

    def __len__(self) -> int: ...

    def __getitem__(self) -> record_entry: ...

    def __name__(self) -> str: ...
