"""
Type stubs file for pypff_folder, defined in [1]

[1] https://github.com/Everlaw/libpff/blob/master/pypff/pypff_folder.c
"""
from .folder import folder
from .item import item
from .items import items
from .message import message


class folder(item):
    ### Instance variables
    # The name.
    name: str

    # The number of sub folders.
    number_of_sub_folders: int

    # The sub folders.
    sub_folders: items

    # The number of sub messages.
    number_of_sub_messages: int

    # The sub messages.
    sub_messages: items

    ### Methods
    def __name__(self) -> str: ...

    def __doc__(self) -> str: ...

    def get_name(self) -> str:
        """
        Retreives the name.
        """
    
    def get_number_of_sub_folders(self) -> int:
        """
        Retrieves the number of sub folders.
        """

    def get_sub_folder(self, sub_folder_index: int) -> folder:
        """
        Retrieves a specific sub folder.
        """
    
    def get_number_of_sub_messages(self) -> int:
        """
        Retrieves the number of sub messages.
        """
    
    def get_sub_message(self, sub_message_index: int) -> message:
        """
        Retrieves a specific sub message.
        """
