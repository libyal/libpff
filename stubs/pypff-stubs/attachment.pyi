"""
Type stubs for pypff_attachment defined in [1]

[1] https://github.com/Everlaw/libpff/blob/master/pypff/pypff_attachment.c
"""
from .item import item


class attachment(item):
    ### Instance variables
    # The data size.
    size: int

    ### Methods
    def __name__(self) -> str: ...

    def get_size(self) -> int:
        """
        Retrieves the attachment data size.
        """
    
    def read_buffer(self, size: int) -> str:
        """
        Reads a buffer of attachment data.
        """
    
    def seek_offset(self, offset: int, whence: int) -> None:
        """
        Seeks an offset within the attachment data.
        """
    
    def get_embedded_attachment(self) -> item:
        """
        Loads an embedded attachment.
        """
