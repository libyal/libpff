dnl Functions for Java bindings
dnl
dnl Version: 20111018

dnl Function to check if java binary is available
dnl kaffe$EXEEXT java$EXEEXT
AC_DEFUN([AX_PROG_JAVA],
 [AC_REQUIRE([AC_EXEEXT])
 AS_IF(
  [test x"$JAVA" = x],
  [AS_IF(
   [test x"$ac_cv_with_jdk" = x],
   [AC_PATH_PROGS(
    [JAVA],
    [java$EXEEXT])],
   [echo $ac_cv_with_jdk
    AC_PATH_PROGS(
    [JAVA],
    [java$EXEEXT],
    [],
    [$ac_cv_with_jdk$PATH_SEPARATOR$ac_cv_with_jdk/bin$PATH_SEPARATOR$ac_cv_with_jdk/jre/bin])
   ])
 AS_IF(
  [test x"$JAVA" = x],
  [AC_MSG_ERROR(
   [Cannot find java$EXEEXT specify the location of the JDK with --with-jdk]) ])
 AC_SUBST(
  [JAVA],
  [$JAVA])
  ])
 ])

dnl Function to check if javac binary is available
dnl "gcj$EXEEXT -C" guavac$EXEEXT jikes$EXEEXT javac$EXEEXT
AC_DEFUN([AX_PROG_JAVAC],
 [AC_REQUIRE([AC_EXEEXT])
 AS_IF(
  [test x"$JAVAC" = x],
  [AS_IF(
   [test x"$ac_cv_with_jdk" = x],
   [AC_PATH_PROGS(
    [JAVAC],
    [javac$EXEEXT]) ],
   [AC_PATH_PROGS(
    [JAVAC],
    [javac$EXEEXT],
    [],
    [$ac_cv_with_jdk$PATH_SEPARATOR$ac_cv_with_jdk/bin$PATH_SEPARATOR$ac_cv_with_jdk/jre/bin])
   ])
 AS_IF(
  [test x"$JAVA" = x],
  [AC_MSG_ERROR(
   [Cannot find javac$EXEEXT specify the location of the JDK with --with-jdk]) ])
 AC_SUBST(
  [JAVAC],
  [$JAVAC])
  ])
 ])

dnl Function to check if javah binary is available
AC_DEFUN([AX_PROG_JAVAH],
 [AC_REQUIRE([AC_EXEEXT])
 AS_IF(
  [test x"$JAVAH" = x],
  [AS_IF(
   [test x"$ac_cv_with_jdk" = x],
   [AC_PATH_PROGS(
    [JAVAH],
    [javah$EXEEXT]) ],
   [AC_PATH_PROGS(
    [JAVAH],
    [javah$EXEEXT],
    [],
    [$ac_cv_with_jdk$PATH_SEPARATOR$ac_cv_with_jdk/bin$PATH_SEPARATOR$ac_cv_with_jdk/jre/bin])
   ])
 AS_IF(
  [test x"$JAVA" = x],
  [AC_MSG_ERROR(
   [Cannot find javah$EXEEXT specify the location of the JDK with --with-jdk]) ])
 AC_SUBST(
  [JAVAH],
  [$JAVAH])
  ])
 ])

dnl Function to check if jar binary is available
AC_DEFUN([AX_PROG_JAR],
 [AC_REQUIRE([AC_EXEEXT])
 AS_IF(
  [test x"$JAR" = x],
  [AS_IF(
   [test x"$ac_cv_with_jdk" = x],
   [AC_PATH_PROGS(
    [JAR],
    [jar$EXEEXT]) ],
   [AC_PATH_PROGS(
    [JAR],
    [jar$EXEEXT],
    [],
    [$ac_cv_with_jdk$PATH_SEPARATOR$ac_cv_with_jdk/bin$PATH_SEPARATOR$ac_cv_with_jdk/jre/bin])
   ])
 AS_IF(
  [test x"$JAVA" = x],
  [AC_MSG_ERROR(
   [Cannot find jar$EXEEXT specify the location of the JDK with --with-jdk]) ])
 AC_SUBST(
  [JAR],
  [$JAR])
  ])
 ])

dnl Function to detect if Java (JNI) build environment is available
AC_DEFUN([AX_CHECK_JAVA_DEVEL],
 [AX_COMMON_ARG_WITH(
  [jdk],
  [jdk],
  [search for JDK in includedir and libdir or in the specified DIR],
  [DIR])

 AC_CHECK_PROG(
  [BASENAME],
  [basename],
  [basename],
  [])
 AC_CHECK_PROG(
  [READLINK],
  [readlink],
  [readlink],
  [])

 AX_PROG_JAVA
 AX_PROG_JAVAC
 AX_PROG_JAVAH
 AX_PROG_JAR

 AC_MSG_CHECKING(
  [for JNI include path])

 dnl Follow java to its original location
 AS_IF(
  [test -n $READLINK],
  [JRE_ROOT_DIR=`$READLINK -f $JAVA`])

 dnl Strip off bin/java
 JRE_ROOT_DIR=`AS_DIRNAME([$JRE_ROOT_DIR])`;
 JRE_ROOT_DIR=`AS_DIRNAME([$JRE_ROOT_DIR])`;

 JNI_INCLUDE_DIR=$JRE_ROOT_DIR;

 dnl Strip off jre if necessary
 AS_IF(
  [test -n $BASENAME && test `$BASENAME $JNI_INCLUDE_DIR` = "jre"],
  [JNI_INCLUDE_DIR=`AS_DIRNAME([$JNI_INCLUDE_DIR])`;])

 dnl Add include
 JNI_INCLUDE_DIR="$JNI_INCLUDE_DIR/include";

 AC_MSG_RESULT(
  [$JNI_INCLUDE_DIR])

 dnl Detect the target platform
 ac_java_target_string="$target";

 AS_IF(
  [test "x$ac_java_target_string" = x],
  [ac_java_target_string="$build"])

 AS_CASE(
  [$ac_java_target_string],
  [*bsdi*], [JNI_PLATFORM_INCLUDE_DIR="$JNI_INCLUDE_DIR/bsdos";],
  [*linux*], [JNI_PLATFORM_INCLUDE_DIR="$JNI_INCLUDE_DIR/linux";],
  [*osf*], [JNI_PLATFORM_INCLUDE_DIR="$JNI_INCLUDE_DIR/alpha";],
  [*solaris*], [JNI_PLATFORM_INCLUDE_DIR="$JNI_INCLUDE_DIR/solaris";],
  [*cygwin*], [JNI_PLATFORM_INCLUDE_DIR="$JNI_INCLUDE_DIR/win32";],
  [*mingw*], [JNI_PLATFORM_INCLUDE_DIR="$JNI_INCLUDE_DIR/win32";],
  [*], [JNI_PLATFORM_INCLUDE_DIR="$JNI_INCLUDE_DIR/genunix";])

 AC_SUBST(
  [JAVA_CPPFLAGS],
  ["-I$JNI_INCLUDE_DIR -I$JNI_PLATFORM_INCLUDE_DIR"])

 AS_IF(
  [test ! -r $JNI_INCLUDE_DIR/jni.h],
  [AC_MSG_ERROR(
  [Missing JNI include file]) ])

 AC_MSG_CHECKING(
  [for JAR package path])

 JAR_PACKAGE_DIR="$JRE_ROOT_DIR/lib/ext";

 AC_MSG_RESULT(
  [$JAR_PACKAGE_DIR])

 AC_SUBST(
  [JAR_PACKAGE_DIR],
  [$JAR_PACKAGE_DIR])

 AC_MSG_CHECKING(
  [for JNI library path])

 dnl Detect the target cpu
 ac_java_target_cpu_string="$target_cpu";

 AS_IF(
  [test "x$ac_java_target_cpu_string" = x],
  [ac_java_target_cpu_string="$build_cpu"])

 AS_CASE(
  [$ac_java_target_cpu_string],
  [i[3-6]86*], [JNI_LIBRARY_DIR="$JRE_ROOT_DIR/lib/i386";],
  [x86_64], [AS_IF(
             [test -d "$JRE_ROOT_DIR/lib/amd64"],
             [JNI_LIBRARY_DIR="$JRE_ROOT_DIR/lib/amd64";],
             [JNI_LIBRARY_DIR="$JRE_ROOT_DIR/lib/$ac_java_target_cpu_string";]) ],
  [*], [JNI_LIBRARY_DIR="$JRE_ROOT_DIR/lib/$ac_java_target_cpu_string";])

 AC_MSG_RESULT(
  [$JNI_LIBRARY_DIR])

 AC_SUBST(
  [JNI_LIBRARY_DIR],
  [$JNI_LIBRARY_DIR])
 ])

dnl Function to detect if to enable Java
AC_DEFUN([AX_JAVA_CHECK_ENABLE],
 [AX_COMMON_ARG_ENABLE(
  [java],
  [java],
  [build Java bindings],
  [no])

 AS_IF(
  [test "x$ac_cv_enable_java" != xno],
  [AX_CHECK_JAVA_DEVEL()
  AC_DEFINE(
   [HAVE_JAVA],
   [1],
   [Define to 1 if Java bindings should be used.])
  ac_cv_enable_java=yes])

 AM_CONDITIONAL(
  HAVE_JAVA,
  [test "x$ac_cv_enable_java" != xno])
 ])

