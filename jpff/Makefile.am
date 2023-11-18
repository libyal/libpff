# Notes:
# If .java files are put in _SOURCES autoconf/make requires GJC
# If .java files are put in _JAVA they will be build after _LTLIBRARIES

JAVA_FILES = \
	Exception.java \
	File.java

if HAVE_JAVA
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@

jardir = @JAR_PACKAGE_DIR@
jnidir = @JNI_LIBRARY_DIR@

jar_DATA = jpff.jar

noinst_JAVA = \
	$(JAVA_FILES)

# On Linux the name of the JNI library should start with lib
# and should be in the library path (ldconfig)
jni_LTLIBRARIES = libjpff.la

BUILT_SOURCES = \
	jpff_File.h

libjpff_la_SOURCES = \
	jpff_File.c \
	jpff_jni.h \
	jpff_libcerror.h \
	jpff_string.c jpff_string.h

libjpff_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libpff/libpff.la

libjpff_la_CPPFLAGS = $(JAVA_CPPFLAGS)
libjpff_la_LDFLAGS  = -module -avoid-version $(JAVA_LDFLAGS)

endif

.java.class:
	$(JAVAC) -classpath .. $(JAVACFLAGS) $<

jpff_File.h: Exception.class File.class
	$(JAVAH) -classpath .. -jni $(JAVAHFLAGS) jpff.File

jpff.jar: $(noinst_JAVA)
	(cd .. && $(JAR) cf $(JARFLAGS) jpff/$@ jpff/*.class)

EXTRA_DIST = \
	$(JAVA_FILES)

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f Makefile

