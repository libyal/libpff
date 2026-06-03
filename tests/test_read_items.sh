#!/usr/bin/env bash
# Read items testing script
#
# Version: 20260603

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

OPTION_SETS=();

INPUT_GLOB="*";

list_contains()
{
	LIST=$1;
	SEARCH=$2;

	for LINE in $LIST;
	do
		if test $LINE = $SEARCH;
		then
			return ${EXIT_SUCCESS};
		fi
	done

	return ${EXIT_FAILURE};
}

TEST_EXECUTABLE="pff_test_read_items";

if ! test -x ${TEST_EXECUTABLE};
then
	TEST_EXECUTABLE="pff_test_read_items.exe";
fi

if ! test -x ${TEST_EXECUTABLE};
then
	echo "Missing executable: ${TEST_EXECUTABLE}";

	exit ${EXIT_FAILURE};
fi

TEST_DIRECTORY=`dirname $0`;

TEST_RUNNER="${TEST_DIRECTORY}/test_runner.sh";

if ! test -f "${TEST_RUNNER}";
then
	echo "Missing test runner: ${TEST_RUNNER}";

	exit ${EXIT_FAILURE};
fi

source ${TEST_RUNNER};

if ! test -d "input";
then
	echo "Test input directory not found.";

	exit ${EXIT_IGNORE};
fi
RESULT=`ls input/* | tr ' ' '\n' | wc -l`;

if test ${RESULT} -eq ${EXIT_SUCCESS};
then
	echo "No files or directories found in the test input directory";

	exit ${EXIT_IGNORE};
fi

local TEST_DESCRIPTION="Testing: read items";

if ! test -d "input";
then
	echo "Test input directory not found.";

	exit ${EXIT_IGNORE};
fi
local RESULT=`ls input/* | tr ' ' '\n' | wc -l`;

if test ${RESULT} -eq ${EXIT_SUCCESS};
then
	echo "No files or directories found in the test input directory";

	exit ${EXIT_IGNORE};
fi

local TEST_PROFILE_DIRECTORY=$(get_test_profile_directory "input" "libpff-read-items");

local IGNORE_LIST=$(read_ignore_list "${TEST_PROFILE_DIRECTORY}");

RESULT=${EXIT_SUCCESS};

for TEST_SET_INPUT_DIRECTORY in input/*;
do
	if ! test -d "${TEST_SET_INPUT_DIRECTORY}";
	then
		continue;
	fi
	if check_for_directory_in_ignore_list "${TEST_SET_INPUT_DIRECTORY}" "${IGNORE_LIST}";
	then
		continue;
	fi

	local TEST_SET_DIRECTORY=$(get_test_set_directory "${TEST_PROFILE_DIRECTORY}" "${TEST_SET_INPUT_DIRECTORY}");

	local INPUT_FILES=()

	if test -f "${TEST_SET_DIRECTORY}/files";
	then
		while IFS= read -r FILENAME;
		do
			if test -n "${FILENAME}}";
			then
				INPUT_FILES+=("${TEST_SET_INPUT_DIRECTORY}/${FILENAME}")
			fi
		done < "${TEST_SET_DIRECTORY}/files"
	else
		for FILENAME in ${TEST_SET_INPUT_DIRECTORY}/${INPUT_GLOB};
		do
			INPUT_FILES+=("${FILENAME}")
		done
	fi
	for INPUT_FILE in "${INPUT_FILES[@]}";
	do
		OPTION_INPUT_FILE="${INPUT_FILE}";

		local TESTED_WITH_OPTIONS=0;

		for OPTION_SET in ${OPTION_SETS[@]};
		do
			local TEST_DATA_OPTION_FILE=$(get_test_data_option_file "${TEST_SET_DIRECTORY}" "${OPTION_INPUT_FILE}" "${OPTION_SET}");

			if test -f ${TEST_DATA_OPTION_FILE};
			then
				TESTED_WITH_OPTIONS=1;

				IFS=" " read -a OPTIONS <<< $(read_test_data_option_file "${TEST_SET_DIRECTORY}" "${INPUT_FILE}" "${OPTION_SET}");

				run_test_on_input_file "${TEST_SET_DIRECTORY}" "${TEST_DESCRIPTION}" "default" "${OPTION_SET}" "${TEST_EXECUTABLE}" "${INPUT_FILE}" "${OPTIONS[@]}";
				RESULT=$?;

				if test ${RESULT} -ne ${EXIT_SUCCESS};
				then
					break;
				fi
			fi
		done

		if test ${TESTED_WITH_OPTIONS} -eq 0;
		then
			run_test_on_input_file "${TEST_SET_DIRECTORY}" "${TEST_DESCRIPTION}" "default" "" "${TEST_EXECUTABLE}" "${INPUT_FILE}";
			RESULT=$?;
		fi

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			break;
		fi
	done

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		break;
	fi
done

exit ${RESULT};

