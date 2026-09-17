#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libpff/.libs/libpff.1.dylib" ] && [ -f ./pypff/.libs/pypff.so ]
then
    install_name_tool -change /usr/local/lib/libpff.1.dylib "${PWD}/libpff/.libs/libpff.1.dylib" ./pypff/.libs/pypff.so
fi

make check-build > /dev/null

# shellcheck disable=SC2068
make check $@
RESULT=$?

if [ ${RESULT} -ne 0 ]
then
    find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

