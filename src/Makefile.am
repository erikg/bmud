# $Id: Makefile.am,v 1.12 2004/01/20 15:29:13 erik Exp $

bin_PROGRAMS = bmud 
bmud_CFLAGS = @PACKAGE_CFLAGS@ @GUILE_CFLAGS@ @PERL_CFLAGS@
bmud_LDADD = @PACKAGE_LIBS@ @GUILE_LIBS@ @PERL_LIBS@

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


