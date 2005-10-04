# $Id: configure.ac,v 1.4 2005/10/04 21:04:44 erik Exp $

AC_INIT(src/main.c)
AM_INIT_AUTOMAKE(bmud, 0.6)
AM_CONFIG_HEADER(config.h)

AC_ISC_POSIX
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_CC_STDC
AC_HEADER_STDC

AC_DEFINE(USE_GNOME,,[Use GNOME Framework])

PKG_CHECK_MODULES(PACKAGE, ["libgnomeui-2.0"])
AC_SUBST(PACKAGE_CFLAGS)
AC_SUBST(PACKAGE_LIBS)

AC_ARG_ENABLE(guile,
[  --enable-guile          Use GNU Guile for scripting [default=yes]],
	, guile=yes)
if test x$guile = xyes; then
	AC_MSG_CHECKING(for Guile)
	AC_DEFINE(USE_GUILE,,[Guile for scripting])
	GUILE_CFLAGS=`guile-config compile`
	GUILE_LIBS=`guile-config link`
	AC_SUBST(GUILE_CFLAGS)
	AC_SUBST(GUILE_LIBS)
	AC_MSG_RESULT(yes)
fi

AC_ARG_ENABLE(python,
[  --enable-python         Use Python for scripting [default=yes]],
	, python=yes)
if test x$python = xyes; then
	AC_MSG_CHECKING(for Python)
	AC_DEFINE(USE_PYTHON,,[python for scripting])
#	PYTHON_CFLAGS=`python -MExtUtils::Embed -e ccopts`
#	PYTHON_LIBS=`python -MExtUtils::Embed -e ldopts`
#	AC_SUBST(PYTHON_CFLAGS)
#	AC_SUBST(PYTHON_LIBS)
	AC_MSG_RESULT(yes)
fi

AC_ARG_ENABLE(perl,
[  --enable-perl           Use PERL for scripting [default=yes]],
	, perl=yes)
if test x$perl = xyes; then
	AC_MSG_CHECKING(for PERL)
	AC_DEFINE(USE_PERL,,[perl for scripting])
	PERL_CFLAGS=`perl -MExtUtils::Embed -e ccopts`
	PERL_LIBS=`perl -MExtUtils::Embed -e ldopts`
	AC_SUBST(PERL_CFLAGS)
	AC_SUBST(PERL_LIBS)
	AC_MSG_RESULT(yes)
fi

AC_OUTPUT(Makefile data/Makefile doc/Makefile src/Makefile bmud.spec bmud.lsm doc/doxygen.conf)
