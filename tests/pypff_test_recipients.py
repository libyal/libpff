#!/usr/bin/env python3
#
# Python-bindings recipients item type test script
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


class RecipientsItemTypeTests(unittest.TestCase):
    """Tests the recipients item type."""

    def _get_recipients_sub_item(self, item):
        """Retrieves the first recipients sub item for testing.

        Returns:
          pypff.recipients: first recipients item or None.
        """
        if item:
            if isinstance(item, pypff.message):
                if item.recipients:
                    return item.recipients

            for sub_item in item.sub_items:
                recipients_item = self._get_recipients_sub_item(sub_item)
                if recipients_item:
                    return recipients_item

        return None

    def test_get_number_of_recipients(self):
        """Tests the get_number_of_recipients function and number_of_recipients property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            message_item = self._get_recipients_sub_item(pff_file.root_folder)
            if not message_item:
                raise unittest.SkipTest("missing recipients item")

            number_of_recipients = message_item.get_number_of_recipients()
            self.assertIsNotNone(number_of_recipients)

            self.assertIsNotNone(message_item.number_of_recipients)

        finally:
            pff_file.close()


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
