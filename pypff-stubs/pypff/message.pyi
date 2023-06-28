"""
Type stubs for pypff_message defined in [1]

[1] https://github.com/Everlaw/libpff/blob/master/pypff/pypff_message.c
"""
import datetime
from .item import item


class message(item):
    ### Instance Variables
    # The subject.
    subject: str

    # The conversation topic.
    conversation_topic: str

    # The sender name.
    sender_name: str

    # The client submit date and time.
    client_submit_time: datetime.datetime

    # The delivery date and time.
    delivery_time: datetime.datetime

    # The creation date and time.
    creation_time: datetime.datetime

    # The modification date and time.
    modification_time: datetime.datetime

    # The transport headers.
    transport_headers: str

    # The plain text body.
    plain_text_body: str

    # The RTF body.
    rtf_body: str

    # The HTML body.
    html_body: str

    ### Methods
    def __name__(self) -> str: ...

    def __doc__(self) -> str: ...

    def get_subject(self) -> str:
        """
        Retrieves the subject.
        """
    
    def get_conversation_topic(self) -> str:
        """
        Retrieves the conversation topic.
        """
    
    def get_sender_name(self) -> str:
        """
        Retrieves the sender name.
        """
    
    def get_client_submit_time(self) -> datetime.datetime:
        """
        Returns the client submit date and time.
        """
    
    def get_client_submit_time_as_integer(self) -> int:
        """
        Returns the client submit date and time as a 64-bit integer containing a FILETIME value.
        """
    
    def get_delivery_time(self) -> datetime.datetime:
        """
        Returns the delivery date and time.
        """

    def get_delivery_time_as_integer(self) -> int:
        """
        Returns the delivery date and time as a 64-bit integer containing a FILETIME value.
        """
    
    def get_creation_time(self) -> datetime.datetime:
        """
        Returns the creation date and time
        """
    
    def get_creation_time_as_integer(self) -> int:
        """
        Returns the creation date and time as a 64-bit integer containing a FILETIME value.
        """
    
    def get_modification_time(self) -> datetime.datetime:
        """
        Returns the modification date and time.
        """
    
    def get_modification_time_as_integer(self) -> int:
        """
        Returns the modification date and time as a 64-bit integer containing a FILETIME value.
        """
    
    def get_transport_headers(self) -> str:
        """
        Retrieves the transport headers.
        """
    
    def get_plain_text_body(self) -> str:
        """
        Retrieves the plain-text body.
        """

    def get_rtf_body(self) -> str:
        """
        Retrieves the RTF body.
        """
    
    def get_html_body(self) -> str:
        """
        Retrieves the HTML body.
        """
