"""
Type stubs for pypff_file, defined in [1]

[1] https://github.com/Everlaw/libpff/blob/master/pypff/pypff_file.c
"""
from typing import IO
from .folder import folder
from .item import item
from .items import items


class file:
    ### Instance variables
    # The size.
    size: int

    # The content type.
    content_type: int

    # The encryption type.
    encryption_type: int

    # The codepage used for ASCII strings in the file.
    ascii_codepage: str

    # The root item.
    root_item: folder

    # The message store.
    message_store: item

    # The name to id map.
    name_to_id_map: item

    # The root folder.
    root_folder: folder

    # The number of orphan items.
    number_of_orphan_items: int

    # The orphan items.
    orphan_items: items

    ### Methods
    def __init__(self) -> None: ...

    def __name__(self) -> str: ...

    def __doc__(self) -> str: ...

    def signal_abort(self) -> None:
        """
        Signals the file to abort the current activity.
        """

    def open(self, filename: str, mode: str = "r") -> None:
        """
        Opens a file.
        """

    def open_file_object(self, file_object: IO, mode: str = "r") -> None:
        """
        Opens a file using a file-like object.
        """

    def close(self) -> None:
        """
        Closes a file.
        """

    def get_size(self) -> int:
        """
        Retrieves the size.
        """

    def get_content_type(self) -> int:
        """
        Retrieves the content type.
        """

    def get_encryption_type(self) -> int:
        """
        Retrieves the encryption type.
        """

    def get_ascii_codepage(self) -> str:
        """
        Retrieves the codepage for ASCII strings used in the file.
        """
    
    def set_ascii_codepage(self, codepage: str) -> None:
        """
        Sets the codepage for ASCII strings used in the file.
        Expects the codepage to be a string containing a Python codec definition.
        """

    def get_root_item(self) -> folder:
        """
        Retrieves the root item.
        """

    def get_message_store(self) -> item:
        """
        Retrieves the message store.
        """

    def get_name_to_id_map(self) -> item:
        """
        Retrieves the name to id map.
        """

    def get_root_folder(self) -> folder:
        """
        Retrieves the root folder.
        """

    def get_number_of_orphan_items(self) -> int:
        """
        Retrieves the number of orphan items.
        """

    def get_orphan_item(self, orphan_item_index: int) -> item:
        """
        Retrieves the orphan item specified by the index.
        """
