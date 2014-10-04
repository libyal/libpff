#!/bin/sh
# Script to generate ./configure using the autotools
#
# Version: 20141004

EXIT_SUCCESS=0;
EXIT_FAILURE=1;

# Default location of MacPorts installed binaries.
BINDIR="/opt/local/bin";

if ! test -d "${BINDIR}";
then
	BINDIR="/usr/bin";
fi

ACLOCAL="${BINDIR}/aclocal";
AUTOCONF="${BINDIR}/autoconf";
AUTOHEADER="${BINDIR}/autoheader";
AUTOMAKE="${BINDIR}/automake";
AUTOPOINT="${BINDIR}/autopoint";
AUTORECONF="${BINDIR}/autoreconf";
LIBTOOLIZE="${BINDIR}/libtoolize";
PKGCONFIG="${BINDIR}/pkg-config";

if ! test -x "${PKGCONFIG}";
then
	echo "Unable to find: pkg-config";

	exit ${EXIT_FAILURE};
fi

if test -x "${AUTORECONF}";
then
	${AUTORECONF} --force --install
else
	if ! test -x "${ACLOCAL}";
	then
		echo "Unable to find: aclocal";

		exit ${EXIT_FAILURE};
	fi

	if ! test -x "${AUTOCONF}";
		then
		echo "Unable to find: autoconf";

		exit ${EXIT_FAILURE};
	fi

	if ! test -x "${AUTOHEADER}";
	then
		echo "Unable to find: autoheader";

		exit ${EXIT_FAILURE};
	fi

	if ! test -x "${AUTOMAKE}";
	then
		echo "Unable to find: automake";

		exit ${EXIT_FAILURE};
	fi

	if ! test -x "${AUTOPOINT}";
	then
		echo "Unable to find: autopoint";

		exit ${EXIT_FAILURE};
	fi

	if ! test -x "${LIBTOOLIZE}";
	then
		echo "Unable to find: libtoolize";

		exit ${EXIT_FAILURE};
	fi

	${AUTOPOINT} --force
	${ACLOCAL} --force -I m4
	${LIBTOOLIZE} --force
	${AUTOHEADER} --force
	${AUTOCONF} --force
	${AUTOMAKE} --force --add-missing
fi

exit ${EXIT_SUCCESS};

