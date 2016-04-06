#!/bin/bash
# Python-bindings set ASCII codepage testing script
#
# Version: 20160126

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_PREFIX=`pwd`;
TEST_PREFIX=`dirname ${TEST_PREFIX}`;
TEST_PREFIX=`basename ${TEST_PREFIX} | sed 's/^lib//'`;

if ! test -z ${SKIP_PYTHON_TESTS};
then
	exit ${EXIT_IGNORE};
fi

PYTHON=`which python${PYTHON_VERSION} 2> /dev/null`;

if ! test -x ${PYTHON};
then
	echo "Missing executable: ${PYTHON}";

	exit ${EXIT_FAILURE};
fi

SCRIPT="py${TEST_PREFIX}_test_set_ascii_codepage.py";

if ! test -f ${SCRIPT};
then
	echo "Missing script: ${SCRIPT}";

	exit ${EXIT_FAILURE};
fi

if test `uname -s` = 'Darwin';
then
	DYLD_LIBRARY_PATH="../lib${TEST_PREFIX}/.libs/" PYTHONPATH="../py${TEST_PREFIX}/.libs/" ${PYTHON} ${SCRIPT};
	RESULT=$?;
else
	LD_LIBRARY_PATH="../lib${TEST_PREFIX}/.libs/" PYTHONPATH="../py${TEST_PREFIX}/.libs/" ${PYTHON} ${SCRIPT};
	RESULT=$?;
fi

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

exit ${EXIT_SUCCESS};

