#!/bin/sh
# Script that synchronizes the local test data

TESTS_INPUT_DIRECTORY="tests/input"
TEST_SET="public"
TEST_FILES="outlook.pst"

mkdir -p "${TESTS_INPUT_DIRECTORY}/.pffexport_recovered"
echo "-mrecovered" > "${TESTS_INPUT_DIRECTORY}/.pffexport_recovered/options"

mkdir -p "${TESTS_INPUT_DIRECTORY}/${TEST_SET}"

for TEST_FILE in ${TEST_FILES}
do
	URL="https://raw.githubusercontent.com/libyal/testdata/refs/heads/main/pst/${TEST_FILE}"
	DESTINATION="${TESTS_INPUT_DIRECTORY}/${TEST_SET}/${TEST_FILE}"
	ATTEMPT=1
	SLEEP=4

	while test ${ATTEMPT} -le 5
	do
		if curl -L -o "${DESTINATION}" ${URL}
		then
			break
		fi
		rm -f "${DESTINATION}"

		if test ${ATTEMPT} -eq 5
		then
			echo "Unable to download: ${TEST_FILE}"

			exit 1
		fi
		sleep ${SLEEP}

		ATTEMPT=`expr ${ATTEMPT} + 1`
		SLEEP=`expr ${SLEEP} \* 2`
	done
done
