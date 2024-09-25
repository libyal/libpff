"""
The stubs file for pypff, a python wrapper for libpff.

We base our understanding of the structure of each class through reading the
class definitions in [1]. 

One peculiarity within libpff that does not translate well into typed Python,
is the fact that all methods and instance variables are accessible for each particular object.
When calling a method or fetching the value of an instance variable, there is no guarantee
whether a value will be returned, or if an exception is raised.

[1] https://github.com/Everlaw/libpff/tree/master/pypff
"""

from .attachment import attachment as attachment
from .file import file as file
from .folder import folder as folder
from .item import item as item
from .items import items as items
from .message import message as message
from .record_entries import record_entries as record_entries
from .record_entry import record_entry as record_entry
from .record_set import record_set as record_set
from .record_sets import record_sets as record_sets
