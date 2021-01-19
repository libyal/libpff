#!/bin/bash
#
# pffexport tool testing script
#
# Copyright (C) 2008-2021, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
#

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

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

test_export()
{ 
	DIRNAME=$1;
	INPUT_FILE=$2;
	BASENAME=`basename ${INPUT_FILE}`;

	if test -d tmp;
	then
		rm -rf tmp;
	fi
	mkdir tmp;

	${TEST_RUNNER} ${PFFEXPORT} -d -m recovered -t tmp/${BASENAME} ${INPUT_FILE};

	RESULT=$?;

	find tmp/${BASENAME}.recovered -type f -exec md5sum {} \; | sort -k 2 > tmp/${BASENAME}.log

	if test -f "input/.pffexport_recovered/${DIRNAME}/${BASENAME}.log.gz";
	then
		zdiff "input/.pffexport_recovered/${DIRNAME}/${BASENAME}.log.gz" "tmp/${BASENAME}.log";

		RESULT=$?;
	else
		mv "tmp/${BASENAME}.log" "input/.pffexport_recovered/${DIRNAME}";

		gzip "input/.pffexport_recovered/${DIRNAME}/${BASENAME}.log";
	fi

	rm -rf tmp;

	echo -n "Testing pffexport of input: ${INPUT_FILE} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

PFFEXPORT="../pfftools/pffexport";

if ! test -x ${PFFEXPORT};
then
	PFFEXPORT="../pfftools/pffexport.exe";
fi

if ! test -x ${PFFEXPORT};
then
	echo "Missing executable: ${PFFEXPORT}";

	exit ${EXIT_FAILURE};
fi

TEST_RUNNER="tests/test_runner.sh";

if ! test -x ${TEST_RUNNER};
then
	TEST_RUNNER="./test_runner.sh";
fi

if ! test -x ${TEST_RUNNER};
then
	echo "Missing test runner: ${TEST_RUNNER}";

	exit ${EXIT_FAILURE};
fi

if ! test -d "input";
then
	echo "No input directory found.";

	exit ${EXIT_IGNORE};
fi

OLDIFS=${IFS};
IFS="
";

RESULT=`ls input/* | tr ' ' '\n' | wc -l`;

if test ${RESULT} -eq 0;
then
	echo "No files or directories found in the input directory.";

	EXIT_RESULT=${EXIT_IGNORE};
else
	IGNORELIST="";

	if ! test -d "input/.pffexport_recovered";
	then
		mkdir "input/.pffexport_recovered";
	fi
	if test -f "input/.pffexport_recovered/ignore";
	then
		IGNORELIST=`cat input/.pffexport_recovered/ignore | sed '/^#/d'`;
	fi
	for TESTDIR in input/*;
	do
		if test -d "${TESTDIR}";
		then
			DIRNAME=`basename ${TESTDIR}`;

			if ! list_contains "${IGNORELIST}" "${DIRNAME}";
			then
				if ! test -d "input/.pffexport_recovered/${DIRNAME}";
				then
					mkdir "input/.pffexport_recovered/${DIRNAME}";
				fi
				if test -f "input/.pffexport_recovered/${DIRNAME}/files";
				then
					TESTFILES=`cat input/.pffexport_recovered/${DIRNAME}/files | sed "s?^?${TESTDIR}/?"`;
				else
					TESTFILES=`ls ${TESTDIR}/*`;
				fi
				for TESTFILE in ${TESTFILES};
				do
					if ! test_export "${DIRNAME}" "${TESTFILE}";
					then
						exit ${EXIT_FAILURE};
					fi
				done
			fi
		fi
	done

	EXIT_RESULT=${EXIT_SUCCESS};
fi

IFS=${OLDIFS};

exit ${EXIT_RESULT};

