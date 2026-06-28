#!/usr/bin/env python3
#
# Python-bindings attachment item type test script
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


class AttachmentItemTypeTests(unittest.TestCase):
    """Tests the attachment item type."""

    def _get_attachment_sub_item(self, item):
        """Retrieves the first attachment sub item for testing.

        Returns:
          pypff.attachment: first attachment item or None.
        """
        if item:
            if isinstance(item, pypff.message):
                if item.number_of_attachments:
                    return item.get_attachment(0)

            for sub_item in item.sub_items:
                attachment_item = self._get_attachment_sub_item(sub_item)
                if attachment_item:
                    return attachment_item

        return None

    def test_get_long_filename(self):
        """Tests the get_long_filename function and long_filename property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            attachment_item = self._get_attachment_sub_item(pff_file.root_folder)
            if not attachment_item:
                raise unittest.SkipTest("missing attachment item")

            _ = attachment_item.get_long_filename()

            _ = attachment_item.long_filename

        finally:
            pff_file.close()

    def test_get_size(self):
        """Tests the get_size function and size property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            attachment_item = self._get_attachment_sub_item(pff_file.root_folder)
            if not attachment_item:
                raise unittest.SkipTest("missing attachment item")

            _ = attachment_item.get_size()

            _ = attachment_item.size

        finally:
            pff_file.close()

    # TODO: add read and seek tests


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
