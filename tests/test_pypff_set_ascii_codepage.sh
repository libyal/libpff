#!/bin/bash
# Python-bindings set ASCII codepage testing script
#
# Version: 20161107

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_PREFIX=`dirname ${PWD}`;
TEST_PREFIX=`basename ${TEST_PREFIX} | sed 's/^lib\([^-]*\).*$/\1/'`;

TEST_SCRIPT="py${TEST_PREFIX}_test_set_ascii_codepage.py";

if ! test -z ${SKIP_PYTHON_TESTS};
then
	exit ${EXIT_IGNORE};
fi

TEST_RUNNER="tests/test_runner.sh";

if ! test -f "${TEST_RUNNER}";
then
	TEST_RUNNER="./test_runner.sh";
fi

if ! test -f "${TEST_RUNNER}";
then
	echo "Missing test runner: ${TEST_RUNNER}";

	exit ${EXIT_FAILURE};
fi

source ${TEST_RUNNER};

TEST_DESCRIPTION="Testing Python-bindings functions: set_ascii_codepage";

run_test_with_arguments "${TEST_DESCRIPTION}" "${TEST_SCRIPT}";
RESULT=$?;

exit ${RESULT};

