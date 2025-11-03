AC_INIT([rhasher], [1.0], [very.little.anna@gmail.com])
AC_CONFIG_SRCDIR([rhasher.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_PROG_CC

AC_CHECK_LIB([rhash], [rhash_library_init],
    [AC_DEFINE([HAVE_RHASH], [1], [Have librhash])
     RHASH_LIBS="-lrhash"],
    [AC_MSG_ERROR([librhash is required but not found])])
AC_CHECK_HEADER([rhash.h], [],
    [AC_MSG_ERROR([rhash.h header not found])])
AC_SUBST([RHASH_LIBS])


AC_ARG_WITH([readline],
    [AS_HELP_STRING([--without-readline],
        [disable readline support @<:@default=check@:>@])],
    [], [with_readline=check])


AS_IF([test "x$with_readline" != xno],
    [AC_CHECK_LIB([readline], [readline], [have_readline_lib=yes], [have_readline_lib=no], [-lncurses])
     AS_IF([test "x$have_readline_lib" = xyes],
        [AC_CHECK_HEADER([readline/readline.h], [have_readline=yes], [have_readline=no])],
        [have_readline=no])
     
     AS_IF([test "x$have_readline" = xyes],
        [AC_DEFINE([HAVE_READLINE], [1], [Define if you have readline])
         LIBREADLINE="-lreadline"
         with_readline=yes],
        [AS_IF([test "x$with_readline" = xcheck],
            [with_readline=no],
            [AC_MSG_FAILURE([readline support requested but not found])])])])

AM_CONDITIONAL([HAVE_READLINE], [test "x$with_readline" = xyes])
AC_SUBST([LIBREADLINE])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo "
Build configuration:
  librhash: yes
  readline: $with_readline
"