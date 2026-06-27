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

	curl -L -o "${TESTS_INPUT_DIRECTORY}/${TEST_SET}/${TEST_FILE}" ${URL}
done
