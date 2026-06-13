#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libpff/.libs/libpff.1.dylib && test -f ./pypff/.libs/pypff.so
then
	install_name_tool -change /usr/local/lib/libpff.1.dylib ${PWD}/libpff/.libs/libpff.1.dylib ./pypff/.libs/pypff.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

