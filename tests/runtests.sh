#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libpff/.libs/libpff.1.dylib && test -f ./pypff/.libs/pypff.so;
then
	install_name_tool -change /usr/local/lib/libpff.1.dylib ${PWD}/libpff/.libs/libpff.1.dylib ./pypff/.libs/pypff.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};

