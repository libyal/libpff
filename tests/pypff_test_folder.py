#!/usr/bin/env python3
#
# Python-bindings folder item type test script
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


class FolderItemTypeTests(unittest.TestCase):
    """Tests the folder item type."""

    def _get_folder_with_messages(self, folder):
        """Retrieves the first folder with messages for testing.

        Args:
          folder (pypff.folder): folder.

        Returns:
          pypff.folder: folder with messages or None.
        """
        if folder:
            if folder.number_of_sub_messages:
                return folder

            for sub_folder in folder.sub_folders:
                folder_with_messages = self._get_folder_with_messages(sub_folder)
                if folder_with_messages:
                    return folder_with_messages

        return None

    def test_get_name(self):
        """Tests the get_name function and name property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            root_folder = pff_file.get_root_folder()
            if not root_folder:
                raise unittest.SkipTest("missing root folder")

            _ = root_folder.get_name()
            _ = root_folder.name

        finally:
            pff_file.close()

    def test_get_number_of_sub_folders(self):
        """Tests the get_number_of_sub_folders function and number_of_sub_folders property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            root_folder = pff_file.get_root_folder()
            if not root_folder:
                raise unittest.SkipTest("missing root folder")

            number_of_sub_folders = root_folder.get_number_of_sub_folders()
            self.assertIsNotNone(number_of_sub_folders)

            self.assertIsNotNone(root_folder.number_of_sub_folders)

        finally:
            pff_file.close()

    def test_get_sub_folder(self):
        """Tests the get_sub_folder function."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            root_folder = pff_file.get_root_folder()
            if not root_folder:
                raise unittest.SkipTest("missing root folder")

            if not root_folder.number_of_sub_folders:
                raise unittest.SkipTest("missing sub folders")

            sub_folder = root_folder.get_sub_folder(0)
            self.assertIsNotNone(sub_folder)

        finally:
            pff_file.close()

    def test_get_sub_folders(self):
        """Tests the sub_folders property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            root_folder = pff_file.get_root_folder()
            if not root_folder:
                raise unittest.SkipTest("missing root folder")

            _ = list(root_folder.sub_folders)

        finally:
            pff_file.close()

    def test_get_number_of_sub_messages(self):
        """Tests the get_number_of_sub_messages function and number_of_sub_messages property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            root_folder = pff_file.get_root_folder()
            if not root_folder:
                raise unittest.SkipTest("missing root folder")

            number_of_sub_messages = root_folder.get_number_of_sub_messages()
            self.assertIsNotNone(number_of_sub_messages)

            self.assertIsNotNone(root_folder.number_of_sub_messages)

        finally:
            pff_file.close()

    def test_get_sub_message(self):
        """Tests the get_sub_message function."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            root_folder = pff_file.get_root_folder()
            if not root_folder:
                raise unittest.SkipTest("missing root folder")

            folder = self._get_folder_with_messages(root_folder)
            if not folder:
                raise unittest.SkipTest("missing folder with messages")

            sub_message = folder.get_sub_message(0)
            self.assertIsNotNone(sub_message)

            # Check if message is initialized correctly.
            _ = sub_message.subject

        finally:
            pff_file.close()

    def test_get_sub_messages(self):
        """Tests the sub_messages property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            root_folder = pff_file.get_root_folder()
            if not root_folder:
                raise unittest.SkipTest("missing root folder")

            _ = list(root_folder.sub_messages)

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
