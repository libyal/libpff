"""
Type stubs for pypff_item, defined in [1]

[1] https://github.com/Everlaw/libpff/blob/master/pypff/pypff_item.c
"""
from typing import Any
from .items import items
from .attachment import attachment
from .record_set import record_set
from .record_sets import record_sets


class item:
    ### Instance variables
    # The identifier.
    identifier: int

    # The number of record sets.
    number_of_record_sets: int

    # The record sets.
    record_sets: record_sets

    # The number of entries.
    number_of_entries: int

    # The number of sub items.
    number_of_sub_items: int

    # The sub items.
    sub_items: items

    # The number of attachments.
    number_of_attachments: int

    # The attachments.
    attachments: items

    ### Methods
    def __init__(self) -> None: ...

    def __name__(self) -> str: ...

    def get_identifier(self) -> int:
        """
        Retrieves the identifier.
        """

    def get_number_of_record_sets(self) -> int:
        """
        Retrieves the number of record sets.
        """

    def get_record_set(self, record_set_index: int) -> record_set:
        """
        Retrieves the record set specified by the index.
        """

    def get_number_of_entries(self) -> int:
        """
        Retrieves the number of entries.
        """

    def get_number_of_sub_items(self) -> int:
        """
        Retrieves the number of sub items.
        """

    def get_recipients(self) -> item:
        """
        Loads Recipients.

        A custom Everlaw pypff binding.
        """

    def get_sub_item(self, sub_item_index: int) -> item:
        """
        Retrieves the sub item specified by the index.
        """

    def get_number_of_attachments(self) -> int:
        """
        Returns the number of attachments.

        A custom Everlaw pypff binding.
        """

    def get_attachment(self, attachment_index: int) -> attachment:
        """
        Retrieves a specific attachment
        """
