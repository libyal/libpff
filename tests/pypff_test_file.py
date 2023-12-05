#!/usr/bin/env python
#
# Python-bindings file type test script
#
# Copyright (C) 2008-2023, Joachim Metz <joachim.metz@gmail.com>
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


class FileTypeTests(unittest.TestCase):
  """Tests the file type."""

  def test_signal_abort(self):
    """Tests the signal_abort function."""
    pff_file = pypff.file()

    pff_file.signal_abort()

  def test_open(self):
    """Tests the open function."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest("missing source")

    pff_file = pypff.file()

    pff_file.open(test_source)

    with self.assertRaises(IOError):
      pff_file.open(test_source)

    pff_file.close()

    with self.assertRaises(TypeError):
      pff_file.open(None)

    with self.assertRaises(ValueError):
      pff_file.open(test_source, mode="w")

  def test_open_file_object(self):
    """Tests the open_file_object function."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest("missing source")

    if not os.path.isfile(test_source):
      raise unittest.SkipTest("source not a regular file")

    pff_file = pypff.file()

    with open(test_source, "rb") as file_object:

      pff_file.open_file_object(file_object)

      with self.assertRaises(IOError):
        pff_file.open_file_object(file_object)

      pff_file.close()

      with self.assertRaises(TypeError):
        pff_file.open_file_object(None)

      with self.assertRaises(ValueError):
        pff_file.open_file_object(file_object, mode="w")

  def test_close(self):
    """Tests the close function."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest("missing source")

    pff_file = pypff.file()

    with self.assertRaises(IOError):
      pff_file.close()

  def test_open_close(self):
    """Tests the open and close functions."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      return

    pff_file = pypff.file()

    # Test open and close.
    pff_file.open(test_source)
    pff_file.close()

    # Test open and close a second time to validate clean up on close.
    pff_file.open(test_source)
    pff_file.close()

    if os.path.isfile(test_source):
      with open(test_source, "rb") as file_object:

        # Test open_file_object and close.
        pff_file.open_file_object(file_object)
        pff_file.close()

        # Test open_file_object and close a second time to validate clean up on close.
        pff_file.open_file_object(file_object)
        pff_file.close()

        # Test open_file_object and close and dereferencing file_object.
        pff_file.open_file_object(file_object)
        del file_object
        pff_file.close()

  def test_set_ascii_codepage(self):
    """Tests the set_ascii_codepage function."""
    supported_codepages = (
        "ascii", "cp874", "cp932", "cp936", "cp949", "cp950", "cp1250",
        "cp1251", "cp1252", "cp1253", "cp1254", "cp1255", "cp1256", "cp1257",
        "cp1258")

    pff_file = pypff.file()

    for codepage in supported_codepages:
      pff_file.set_ascii_codepage(codepage)

    unsupported_codepages = (
        "iso-8859-1", "iso-8859-2", "iso-8859-3", "iso-8859-4", "iso-8859-5",
        "iso-8859-6", "iso-8859-7", "iso-8859-8", "iso-8859-9", "iso-8859-10",
        "iso-8859-11", "iso-8859-13", "iso-8859-14", "iso-8859-15",
        "iso-8859-16", "koi8_r", "koi8_u")

    for codepage in unsupported_codepages:
      with self.assertRaises(RuntimeError):
        pff_file.set_ascii_codepage(codepage)

  def test_get_size(self):
    """Tests the get_size function and size property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest("missing source")

    pff_file = pypff.file()

    pff_file.open(test_source)

    size = pff_file.get_size()
    self.assertIsNotNone(size)

    self.assertIsNotNone(pff_file.size)

    pff_file.close()

  def test_get_content_type(self):
    """Tests the get_content_type function and content_type property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest("missing source")

    pff_file = pypff.file()

    pff_file.open(test_source)

    content_type = pff_file.get_content_type()
    self.assertIsNotNone(content_type)

    self.assertIsNotNone(pff_file.content_type)

    pff_file.close()

  def test_get_encryption_type(self):
    """Tests the get_encryption_type function and encryption_type property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest("missing source")

    pff_file = pypff.file()

    pff_file.open(test_source)

    encryption_type = pff_file.get_encryption_type()
    self.assertIsNotNone(encryption_type)

    self.assertIsNotNone(pff_file.encryption_type)

    pff_file.close()

  def test_get_ascii_codepage(self):
    """Tests the get_ascii_codepage function and ascii_codepage property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest("missing source")

    pff_file = pypff.file()

    pff_file.open(test_source)

    ascii_codepage = pff_file.get_ascii_codepage()
    self.assertIsNotNone(ascii_codepage)

    self.assertIsNotNone(pff_file.ascii_codepage)

    pff_file.close()

  def test_get_root_item(self):
    """Tests the get_root_item function and root_item property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest("missing source")

    pff_file = pypff.file()

    pff_file.open(test_source)

    _ = pff_file.get_root_item()

    _ = pff_file.root_item

    pff_file.close()

  def test_get_message_store(self):
    """Tests the get_message_store function and message_store property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest("missing source")

    pff_file = pypff.file()

    pff_file.open(test_source)

    _ = pff_file.get_message_store()

    _ = pff_file.message_store

    pff_file.close()

  def test_get_name_to_id_map(self):
    """Tests the get_name_to_id_map function and name_to_id_map property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest("missing source")

    pff_file = pypff.file()

    pff_file.open(test_source)

    _ = pff_file.get_name_to_id_map()

    _ = pff_file.name_to_id_map

    pff_file.close()

  def test_get_root_folder(self):
    """Tests the get_root_folder function and root_folder property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest("missing source")

    pff_file = pypff.file()

    pff_file.open(test_source)

    _ = pff_file.get_root_folder()

    _ = pff_file.root_folder

    pff_file.close()

  def test_get_number_of_orphan_items(self):
    """Tests the get_number_of_orphan_items function and number_of_orphan_items property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest("missing source")

    pff_file = pypff.file()

    pff_file.open(test_source)

    number_of_orphan_items = pff_file.get_number_of_orphan_items()
    self.assertIsNotNone(number_of_orphan_items)

    self.assertIsNotNone(pff_file.number_of_orphan_items)

    pff_file.close()


if __name__ == "__main__":
  argument_parser = argparse.ArgumentParser()

  argument_parser.add_argument(
      "source", nargs="?", action="store", metavar="PATH",
      default=None, help="path of the source file.")

  options, unknown_options = argument_parser.parse_known_args()
  unknown_options.insert(0, sys.argv[0])

  setattr(unittest, "source", options.source)

  unittest.main(argv=unknown_options, verbosity=2)
