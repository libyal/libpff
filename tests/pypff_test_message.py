#!/usr/bin/env python3
#
# Python-bindings message item type test script
#
# Copyright (C) 2008-2026, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import argparse
import os
import sys
import unittest

import pypff


class MessageItemTypeTests(unittest.TestCase):
    """Tests the message item type."""

    def _get_message_item(self):
        """Retrieves the first message item for testing.

        Returns:
          pypff.message: first message item or None.
        """
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            root_folder = pff_file.get_root_folder()
            if not root_folder:
                raise unittest.SkipTest("missing root folder")

            if root_folder.number_of_sub_items:
                message_item = self._get_message_sub_item(root_folder)
                if message_item:
                    return message_item

        finally:
            pff_file.close()

        return None

    def _get_message_sub_item(self, item):
        """Retrieves the first message sub item for testing.

        Returns:
          pypff.message: first message item or None.
        """
        if item:
            if isinstance(item, pypff.message):
                return item

            for sub_item in item.sub_items:
                message_item = self._get_message_sub_item(sub_item)
                if message_item:
                    return message_item

        return None

    def test_get_subject(self):
        """Tests the get_subject function and subject property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            message_item = self._get_message_sub_item(pff_file.root_folder)
            if not message_item:
                raise unittest.SkipTest("missing message item")

            _ = message_item.get_subject()

            _ = message_item.subject

        finally:
            pff_file.close()

        return None

    def test_get_conversation_topic(self):
        """Tests the get_conversation_topic function and conversation_topic property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            message_item = self._get_message_sub_item(pff_file.root_folder)
            if not message_item:
                raise unittest.SkipTest("missing message item")

            _ = message_item.get_conversation_topic()

            _ = message_item.conversation_topic

        finally:
            pff_file.close()

        return None

    def test_get_sender_name(self):
        """Tests the get_sender_name function and sender_name property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            message_item = self._get_message_sub_item(pff_file.root_folder)
            if not message_item:
                raise unittest.SkipTest("missing message item")

            _ = message_item.get_sender_name()

            _ = message_item.sender_name

        finally:
            pff_file.close()

        return None

    def test_get_client_submit_time(self):
        """Tests the get_client_submit_time function and client_submit_time property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            message_item = self._get_message_sub_item(pff_file.root_folder)
            if not message_item:
                raise unittest.SkipTest("missing message item")

            _ = message_item.get_client_submit_time()
            _ = message_item.get_client_submit_time_as_integer()

            _ = message_item.client_submit_time

        finally:
            pff_file.close()

        return None

    def test_get_delivery_time(self):
        """Tests the get_delivery_time function and delivery_time property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            message_item = self._get_message_sub_item(pff_file.root_folder)
            if not message_item:
                raise unittest.SkipTest("missing message item")

            _ = message_item.get_delivery_time()
            _ = message_item.get_delivery_time_as_integer()

            _ = message_item.delivery_time

        finally:
            pff_file.close()

        return None

    def test_get_creation_time(self):
        """Tests the get_creation_time function and creation_time property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            message_item = self._get_message_sub_item(pff_file.root_folder)
            if not message_item:
                raise unittest.SkipTest("missing message item")

            _ = message_item.get_creation_time()
            _ = message_item.get_creation_time_as_integer()

            _ = message_item.creation_time

        finally:
            pff_file.close()

        return None

    def test_get_modification_time(self):
        """Tests the get_modification_time function and modification_time property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            message_item = self._get_message_sub_item(pff_file.root_folder)
            if not message_item:
                raise unittest.SkipTest("missing message item")

            _ = message_item.get_modification_time()
            _ = message_item.get_modification_time_as_integer()

            _ = message_item.modification_time

        finally:
            pff_file.close()

        return None

    def test_get_transport_headers(self):
        """Tests the get_transport_headers function and transport_headers property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            message_item = self._get_message_sub_item(pff_file.root_folder)
            if not message_item:
                raise unittest.SkipTest("missing message item")

            _ = message_item.get_transport_headers()

            _ = message_item.transport_headers

        finally:
            pff_file.close()

        return None

    def test_get_plain_text_body(self):
        """Tests the get_plain_text_body function and plain_text_body property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            message_item = self._get_message_sub_item(pff_file.root_folder)
            if not message_item:
                raise unittest.SkipTest("missing message item")

            _ = message_item.get_plain_text_body()

            _ = message_item.plain_text_body

        finally:
            pff_file.close()

        return None

    def test_get_rtf_body(self):
        """Tests the get_rtf_body function and rtf_body property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            message_item = self._get_message_sub_item(pff_file.root_folder)
            if not message_item:
                raise unittest.SkipTest("missing message item")

            _ = message_item.get_rtf_body()

            _ = message_item.rtf_body

        finally:
            pff_file.close()

        return None

    def test_get_html_body(self):
        """Tests the get_html_body function and html_body property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            message_item = self._get_message_sub_item(pff_file.root_folder)
            if not message_item:
                raise unittest.SkipTest("missing message item")

            _ = message_item.get_html_body()

            _ = message_item.html_body

        finally:
            pff_file.close()

        return None

    def test_get_number_of_attachments(self):
        """Tests the get_number_of_attachments function and number_of_attachments property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            message_item = self._get_message_sub_item(pff_file.root_folder)
            if not message_item:
                raise unittest.SkipTest("missing message item")

            number_of_attachments = message_item.get_number_of_attachments()
            self.assertIsNotNone(number_of_attachments)

            self.assertIsNotNone(message_item.number_of_attachments)

        finally:
            pff_file.close()

        return None

    def test_get_attachment(self):
        """Tests the get_attachment function."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            message_item = self._get_message_sub_item(pff_file.root_folder)
            if not message_item:
                raise unittest.SkipTest("missing message item")

            if not message_item.number_of_attachments:
                raise unittest.SkipTest("missing attachments")

            attachment = message_item.get_attachment(0)
            self.assertIsNotNone(attachment)

        finally:
            pff_file.close()

        return None


if __name__ == "__main__":
    argument_parser = argparse.ArgumentParser()

    argument_parser.add_argument(
        "source",
        nargs="?",
        action="store",
        metavar="PATH",
        default=None,
        help="path of the source file.",
    )
    options, unknown_options = argument_parser.parse_known_args()
    unknown_options.insert(0, sys.argv[0])

    setattr(unittest, "source", options.source)

    unittest.main(argv=unknown_options, verbosity=2)
