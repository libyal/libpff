#!/usr/bin/env python3
#
# Python-bindings module type test script
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

import unittest

import pypff


class ModuleTypeTests(unittest.TestCase):
    """Tests the Python types exposed by the module."""

    def test_specialized_types(self):
        """Tests the specialized item and record-entry value types."""
        self.assertTrue(issubclass(pypff.message_store, pypff.item))
        self.assertTrue(issubclass(pypff.task, pypff.message))
        self.assertTrue(issubclass(pypff.recipients, pypff.item))
        self.assertIsInstance(pypff.multi_value, type)
        self.assertIsInstance(pypff.name_to_id_map_entry, type)


if __name__ == "__main__":
    unittest.main(verbosity=2)
