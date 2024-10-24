"""
Type stubs file for pypff_items, defined in [1]

[1] https://github.com/Everlaw/libpff/blob/master/pypff/pypff_items.c
"""
from typing import Iterator
from .item import item


class items:
    ### Methods
    def __init__(self) -> None: ...

    def __iter__(self) -> Iterator[item]: ...

    def __next__(self) -> item: ...

    def __len__(self) -> int: ...

    def __getitem__(self) -> item: ...

    def __name__(self) -> str: ...
