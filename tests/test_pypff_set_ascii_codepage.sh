#!/bin/bash
#
# Python-bindings set ASCII codepage testing script
#
# Copyright (C) 2008-2015, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
#

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

PYTHON=`which python${PYTHON_VERSION} 2> /dev/null`;

if ! test -x ${PYTHON};
then
	echo "Missing executable: ${PYTHON}";

	exit ${EXIT_FAILURE};
fi

SCRIPT="pypff_test_set_ascii_codepage.py";

if ! test -f ${SCRIPT};
then
	echo "Missing script: ${SCRIPT}";

	exit ${EXIT_FAILURE};
fi

if test `uname -s` = 'Darwin';
then
	DYLD_LIBRARY_PATH="../libpff/.libs/" PYTHONPATH="../pypff/.libs/" ${PYTHON} ${SCRIPT};
	RESULT=$?;
else
	LD_LIBRARY_PATH="../libpff/.libs/" PYTHONPATH="../pypff/.libs/" ${PYTHON} ${SCRIPT};
	RESULT=$?;
fi

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

exit ${EXIT_SUCCESS};

