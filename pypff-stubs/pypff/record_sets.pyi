"""
Type stubs for pypff_record_sets, defined in [1]

[1] https://github.com/Everlaw/libpff/blob/master/pypff/pypff_record_sets.c
"""
from typing import Iterator
from .record_set import record_set


class record_sets:
    ### Methods
    def __init__(self) -> None: ...

    def __iter__(self) -> Iterator[record_set]: ...

    def __next__(self) -> record_set: ...

    def __len__(self) -> int: ...

    def __getitem__(self) -> record_set: ...

    def __name__(self) -> str: ...

    def __doc__(self) -> str: ...
