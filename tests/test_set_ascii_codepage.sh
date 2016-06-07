#!/bin/bash
# Library set ASCII codepage testing script
#
# Version: 20160328

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_PREFIX=`dirname ${PWD}`;
TEST_PREFIX=`basename ${TEST_PREFIX} | sed 's/^lib\([^-]*\).*$/\1/'`;

TEST_PROFILE="lib${TEST_PREFIX}";
TEST_DESCRIPTION="set ASCII codepage";
OPTION_SETS="";

TEST_TOOL_DIRECTORY=".";
TEST_TOOL="${TEST_PREFIX}_test_set_ascii_codepage";

test_set_supported_ascii_codepage()
{
	local TEST_EXECUTABLE=$1;
	local CODEPAGE=$2;

	echo -n -e "Testing setting supported ASCII codepage: ${CODEPAGE}\t"

	run_test_with_arguments ${TEST_EXECUTABLE} ${CODEPAGE};
	local RESULT=$?;

	if test ${RESULT} -ne 0;
	then
		echo "(FAIL)";
	else
		echo "(PASS)";
	fi
	return ${RESULT};
}

test_set_unsupported_ascii_codepage()
{
	local TEST_EXECUTABLE=$1;
	local CODEPAGE=$2;

	echo -n -e "Testing setting unsupported ASCII codepage: ${CODEPAGE}\t"

	run_test_with_arguments ${TEST_EXECUTABLE} ${CODEPAGE};
	local RESULT=$?;

	if test ${RESULT} -eq 0;
	then
		echo "(FAIL)";

		RESULT=${EXIT_FAILURE};
	else
		echo "(PASS)";

		RESULT=${EXIT_SUCCESS};
	fi
	return ${RESULT};
}

if ! test -z ${SKIP_LIBRARY_TESTS};
then
	exit ${EXIT_IGNORE};
fi

TEST_EXECUTABLE="${TEST_TOOL_DIRECTORY}/${TEST_TOOL}";

if ! test -x "${TEST_EXECUTABLE}";
then
	TEST_EXECUTABLE="${TEST_TOOL_DIRECTORY}/${TEST_TOOL}.exe";
fi

if ! test -x "${TEST_EXECUTABLE}";
then
	echo "Missing test executable: ${TEST_EXECUTABLE}";

	exit ${EXIT_FAILURE};
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

CODEPAGES="ascii windows-874 windows-932 windows-936 windows-949 windows-950 windows-1250 windows-1251 windows-1252 windows-1253 windows-1254 windows-1255 windows-1256 windows-1257 windows-1258";

for CODEPAGE in ${CODEPAGES};
do
	test_set_supported_ascii_codepage "${TEST_EXECUTABLE}" "${CODEPAGE}";
	RESULT=$?;

	if test ${RESULT} -ne 0;
	then
		exit ${RESULT};
	fi
done

CODEPAGES="iso-8859-1 iso-8859-2 iso-8859-3 iso-8859-4 iso-8859-5 iso-8859-6 iso-8859-7 iso-8859-8 iso-8859-9 iso-8859-10 iso-8859-11 iso-8859-13 iso-8859-14 iso-8859-15 iso-8859-16 koi8_r koi8_u";

for CODEPAGE in ${CODEPAGES};
do
	test_set_unsupported_ascii_codepage "${TEST_EXECUTABLE}" "${CODEPAGE}";
	RESULT=$?;

	if test ${RESULT} -ne 0;
	then
		exit ${RESULT};
	fi
done

exit ${RESULT};

