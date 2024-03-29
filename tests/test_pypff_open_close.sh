#!/usr/bin/env bash
# Python-bindings open close testing script
#
# Version: 20230410

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_PREFIX=`pwd`;
TEST_PREFIX=`dirname ${TEST_PREFIX}`;
TEST_PREFIX=`basename ${TEST_PREFIX} | sed 's/^lib//'`;

TEST_SCRIPT="py${TEST_PREFIX}_test_open_close.py";
OPTION_SETS="";
INPUT_GLOB="*";

list_contains()
{
	LIST=$1;
	SEARCH=$2;

	for LINE in ${LIST};
	do
		if test ${LINE} = ${SEARCH};
		then
			return ${EXIT_SUCCESS};
		fi
	done

	return ${EXIT_FAILURE};
}

run_test()
{ 
	TEST_SET_DIR=$1;
	TEST_DESCRIPTION=$2;
	TEST_EXECUTABLE=$3;
	INPUT_FILE=$4;
	OPTION_SET=$5;

	INPUT_NAME=`basename ${INPUT_FILE}`;

	if test -z "${OPTION_SET}";
	then
		OPTIONS="";
	else
		OPTIONS=`cat "${TEST_SET_DIR}/${INPUT_NAME}.${OPTION_SET}" | head -n 1 | sed 's/[\r\n]*$//'`;
	fi
	TMPDIR="tmp$$";

	rm -rf ${TMPDIR};
	mkdir ${TMPDIR};

	if test -z "${OPTION_SET}";
	then
		echo "Testing ${TEST_DESCRIPTION} with input: ${INPUT_FILE}";
	else
		echo "Testing ${TEST_DESCRIPTION} with option: ${OPTION_SET} and input: ${INPUT_FILE}";
	fi

	if test `uname -s` = 'Darwin';
	then
		DYLD_LIBRARY_PATH="../lib${TEST_PREFIX}/.libs/" PYTHONPATH="../py${TEST_PREFIX}/.libs/" ${PYTHON} ${TEST_SCRIPT} ${OPTIONS} ${INPUT_FILE};
		RESULT=$?;
	else
		LD_LIBRARY_PATH="../lib${TEST_PREFIX}/.libs/" PYTHONPATH="../py${TEST_PREFIX}/.libs/" ${PYTHON} ${TEST_SCRIPT} ${OPTIONS} ${INPUT_FILE};
		RESULT=$?;
	fi

	RESULT=$?;

	rm -rf ${TMPDIR};

	echo "";

	return ${RESULT};
}

run_tests()
{
	TEST_PROFILE=$1;
	TEST_DESCRIPTION=$2;
	TEST_EXECUTABLE=$3;

	if ! test -d "input";
	then
		echo "No input directory found.";

		return ${EXIT_IGNORE};
	fi
	RESULT=`ls input/* | tr ' ' '\n' | wc -l`;

	if test ${RESULT} -eq 0;
	then
		echo "No files or directories found in the input directory.";

		return ${EXIT_IGNORE};
	fi
	TEST_PROFILE_DIR="input/.${TEST_PROFILE}";

	if ! test -d "${TEST_PROFILE_DIR}";
	then
		mkdir ${TEST_PROFILE_DIR};
	fi
	IGNORE_FILE="${TEST_PROFILE_DIR}/ignore";
	IGNORE_LIST="";

	if test -f "${IGNORE_FILE}";
	then
		IGNORE_LIST=`cat ${IGNORE_FILE} | sed '/^#/d'`;
	fi

	for INPUT_DIR in input/*;
	do
		if ! test -d "${INPUT_DIR}";
		then
			continue
		fi
		INPUT_NAME=`basename ${INPUT_DIR}`;

		if list_contains "${IGNORE_LIST}" "${INPUT_NAME}";
		then
			continue
		fi
		TEST_SET_DIR="${TEST_PROFILE_DIR}/${INPUT_NAME}";

		if ! test -d "${TEST_SET_DIR}";
		then
			mkdir "${TEST_SET_DIR}";
		fi

		if test -f "${TEST_SET_DIR}/files";
		then
			INPUT_FILES=`cat ${TEST_SET_DIR}/files | sed "s?^?${INPUT_DIR}/?"`;
		else
			INPUT_FILES=`ls ${INPUT_DIR}/${INPUT_GLOB}`;
		fi

		for INPUT_FILE in ${INPUT_FILES};
		do
			TESTED_WITH_OPTIONS=0;
			INPUT_NAME=`basename ${INPUT_FILE}`;

			for OPTION_SET in `echo ${OPTION_SETS} | tr ' ' '\n'`;
			do
				OPTION_FILE="${TEST_SET_DIR}/${INPUT_NAME}.${OPTION_SET}";

				if ! test -f "${OPTION_FILE}";
				then
					continue
				fi

				if ! run_test "${TEST_SET_DIR}" "${TEST_DESCRIPTION}" "${TEST_EXECUTABLE}" "${INPUT_FILE}" "${OPTION_SET}";
				then
					return ${EXIT_FAILURE};
				fi
				TESTED_WITH_OPTIONS=1;
			done

			if test ${TESTED_WITH_OPTIONS} -eq 0;
			then
				if ! run_test "${TEST_SET_DIR}" "${TEST_DESCRIPTION}" "${TEST_EXECUTABLE}" "${INPUT_FILE}" "";
				then
					return ${EXIT_FAILURE};
				fi
			fi
		done
	done

	return ${EXIT_SUCCESS};
}

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

if ! test -f ${TEST_SCRIPT};
then
	echo "Missing script: ${TEST_SCRIPT}";

	exit ${EXIT_FAILURE};
fi

OLDIFS=${IFS};
IFS="
";

run_tests "py${TEST_PREFIX}" "open_close" "${TEST_SEEK}";

RESULT=$?;

IFS=${OLDIFS};

exit ${RESULT};

