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

    # TODO: add tests for sub_folders

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

            if not root_folder.number_of_sub_messages:
                raise unittest.SkipTest("missing sub messages")

            sub_message = root_folder.get_sub_message(0)
            self.assertIsNotNone(sub_message)

        finally:
            pff_file.close()

    # TODO: add tests for sub_messages


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
