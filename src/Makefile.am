# $Id: Makefile.am,v 1.15 2005/10/04 21:22:32 erik Exp $

bin_PROGRAMS = bmud 
AM_CFLAGS = @PACKAGE_CFLAGS@ @GUILE_CFLAGS@ @PERL_CFLAGS@
LDADD = @PACKAGE_LIBS@ @GUILE_LIBS@ @PERL_LIBS@

GUI=gnome

bmud_sources = alias.c alias_utils.c color.c conf.c cwiz.c gui.c main.c misc.c net.c prefs.c script.c
noinst_HEADERS = $(bmud_sources:.c=.h) bmud.h g-compat.h

bmud_SOURCES = $(BMUD_SOURCES) gnome-compat.c

#if GNOME
#bmud_SOURCES += gnome-compat.c
#else
#bmud_SOURCES += gtk-compat.c
#endif

lint:
	for a in *.[ch]; do echo $$a ; lint $(CFLAGS) +posixstrictlib $$a > $$a.lint; done
indent:
	indent *.[ch]

splint:
	for a in *.[ch]; do echo $$a ; splint $(CFLAGS) +posixstrictlib $$a > $$a.lint; done
gindent:
	gindent *.[ch]


