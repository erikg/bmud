# $Id: Makefile.am,v 1.11 2004/01/19 17:24:36 erik Exp $

bin_PROGRAMS = bmud 
CFLAGS = @PACKAGE_CFLAGS@ @GUILE_CFLAGS@ @PERL_CFLAGS@
LDADD = @PACKAGE_LIBS@ @GUILE_LIBS@ @PERL_LIBS@

bmud_SOURCES = alias.c alias_utils.c color.c conf.c cwiz.c gui.c main.c misc.c net.c prefs.c script.c
noinst_HEADERS = $(bmud_SOURCES:.c=.h) bmud.h gnome-compat.h gtk-compat.h authors.h

lint:
	for a in *.[ch]; do echo $$a ; lint $(CFLAGS) +posixstrictlib $$a > $$a.lint; done
indent:
	indent *.[ch]

splint:
	for a in *.[ch]; do echo $$a ; splint $(CFLAGS) +posixstrictlib $$a > $$a.lint; done
gindent:
	gindent *.[ch]


