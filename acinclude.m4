dnl Function to detect if libpff dependencies are available
AC_DEFUN([AX_LIBPFF_CHECK_LOCAL],
 [dnl Check for internationalization functions in libpff/libpff_i18n.c 
 AC_CHECK_FUNCS([bindtextdomain])
 ])

