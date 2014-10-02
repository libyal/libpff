dnl Functions for zlib
dnl
dnl Version: 20130402

dnl Function to detect if zlib is available
AC_DEFUN([AX_ZLIB_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_zlib" != x && test "x$ac_cv_with_zlib" != xno && test "x$ac_cv_with_zlib" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_zlib"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_zlib}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_zlib}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_zlib])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_zlib" = xno],
  [ac_cv_zlib=no],
  [dnl Check for a pkg-config file
  AS_IF(
   [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
   [PKG_CHECK_MODULES(
    [zlib],
    [zlib >= 1.2.5],
    [ac_cv_zlib=zlib],
    [ac_cv_zlib=no])
   ])

  AS_IF(
   [test "x$ac_cv_zlib" = xzlib],
   [ac_cv_zlib_CPPFLAGS="$pkg_cv_zlib_CFLAGS"
   ac_cv_zlib_LIBADD="$pkg_cv_zlib_LIBS"],
   [dnl Check for headers
   AC_CHECK_HEADERS([zlib.h])

   AS_IF(
    [test "x$ac_cv_header_zlib_h" = xno],
    [ac_cv_zlib=no],
    [dnl Check for the individual functions
    ac_cv_zlib=zlib

    AC_CHECK_LIB(
     z,
     uncompress,
     [],
     [ac_cv_zlib=no])

    AS_IF(
     [test "x$ac_cv_lib_z_uncompress" = xno],
     [AC_MSG_FAILURE(
      [Missing function: uncompress in library: zlib.],
      [1])
     ])

    ac_cv_zlib_LIBADD="-lz";
    ])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_zlib" = xzlib],
  [AC_CHECK_LIB(
   z,
   compressBound,
   [ac_zlib_dummy=yes])

  AS_IF(
   [test "x$ac_cv_lib_z_compressBound" = xyes],
   [AC_DEFINE(
    [HAVE_COMPRESS_BOUND],
    [1],
    [Define to 1 if compressBound funtion is available in zlib.])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_zlib" = xzlib],
  [AC_DEFINE(
   [HAVE_ZLIB],
   [1],
   [Define to 1 if you have the 'zlib' library (-lz).])
  ])

 AS_IF(
  [test "x$ac_cv_zlib" != xno],
  [AC_SUBST(
   [HAVE_ZLIB],
   [1]) ],
  [AC_SUBST(
   [HAVE_ZLIB],
   [0])
  ])
 ])

dnl Function to detect how to enable zlib
AC_DEFUN([AX_ZLIB_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [zlib],
  [zlib],
  [search for zlib in includedir and libdir or in the specified DIR, or no if not to use zlib],
  [auto-detect],
  [DIR])

 dnl Check for a shared library version
 AX_ZLIB_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_zlib_CPPFLAGS" != "x"],
  [AC_SUBST(
   [ZLIB_CPPFLAGS],
   [$ac_cv_zlib_CPPFLAGS])
  ])
 AS_IF(
  [test "x$ac_cv_zlib_LIBADD" != "x"],
  [AC_SUBST(
   [ZLIB_LIBADD],
   [$ac_cv_zlib_LIBADD])
  ])

 AS_IF(
  [test "x$ac_cv_zlib" = xzlib],
  [AC_SUBST(
   [ax_zlib_pc_libs_private],
   [-lz])
  ])

 AS_IF(
  [test "x$ac_cv_zlib" = xzlib],
  [AC_SUBST(
   [ax_zlib_spec_requires],
   [zlib])
  AC_SUBST(
   [ax_zlib_static_spec_requires],
   [zlib-static])
  AC_SUBST(
   [ax_zlib_spec_build_requires],
   [zlib-devel])
  ])
 ])

