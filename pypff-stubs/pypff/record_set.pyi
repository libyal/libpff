"""
Type stubs for pypff_record_set, defined in [1]

[1] https://github.com/Everlaw/libpff/blob/master/pypff/pypff_record_set.c
"""
from .record_entries import record_entries
from .record_entry import record_entry


class record_set:
    ### Instance Variables
    # The number of entries.
    number_of_entries: int

    # The entries.
    entries: record_entries

    ### Methods
    def __init__(self) -> None: ...

    def __name__(self) -> str: ...

    def get_number_of_entries(self) -> int:
        """
        Retrieves the number of entries.
        """

    def get_entry(self, entry_index: int) -> record_entry:
        """
        Retrieves the entry specified by the index.
        """
