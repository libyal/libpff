"""
Type stubs for pypff_record_entry, defined in [1]

[1] https://github.com/Everlaw/libpff/blob/master/pypff/pypff_record_entry.c
"""
import datetime


class record_entry:
    ### Instance variables
    # The entry type.
    entry_type: int

    # The value type.
    value_type: int

    # The data.
    data: bytes

    # The data as a boolean.
    data_as_boolean: bool

    # The data as an integer.
    data_as_integer: int

    # The data as a datetime object.
    data_as_datetime: datetime.datetime

    # The data as size.
    data_as_size: int

    # The data as a floating point.
    data_as_floating_point: float

    # The data as a string.
    data_as_string: str

    ### Methods
    def __init__(self) -> None: ...

    def __name__(self) -> str: ...

    def __doc__(self) -> str: ...

    def get_entry_type(self) -> int:
        """
        Retrieves the entry type.
        """

    def get_value_type(self) -> int:
        """
        Retrieves the value type.
        """
    
    def get_data(self) -> bytes:
        """
        Retrieves the data.
        """

    def get_data_as_boolean(self) -> bool:
        """
        Retrieves the data as a boolean.
        """

    def get_data_as_integer(self) -> int:
        """
        Retrieves the data as an integer.
        """
    
    def get_data_as_datetime(self) -> datetime.datetime:
        """
        Retrieves the data as a datetime object.
        """

    def get_data_as_size(self) -> int:
        """
        Retrieves the data as size.
        """
    
    def get_data_as_floating_point(self) -> float:
        """
        Retrieves the data as a floating point.
        """
    
    def get_data_as_string(self) -> str:
        """
        Retrieves the data as a string.
        """
