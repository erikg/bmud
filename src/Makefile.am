# $Id: Makefile.am,v 1.6 2004/01/04 18:49:47 erik Exp $

INCLUDES = \
        -DPACKAGE_DATA_DIR=\""$(datadir)"\" \
        -DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
        @PACKAGE_CFLAGS@


bin_PROGRAMS = bmud 
bmud_LDADD = @PACKAGE_LIBS@

bmud_SOURCES = \
	alias.c\
	alias_utils.c\
	color.c\
	conf.c\
	cwiz.c\
	gui.c\
	main.c\
	misc.c\
	net.c\
	prefs.c\
	script.c

noinst_HEADERS = \
	alias.h\
	bmud.h\
	color.h\
	conf.h\
	gnome-compat.h\
	gtk-compat.h\
	gui.h\
	main.h\
	misc.h\
	net.h\
	prefs.h\
	script.h

lint:
	for a in *.[ch]; do echo $$a ; lint $(CFLAGS) +posixstrictlib $$a > $$a.lint; done
indent:
	indent *.[ch]

splint:
	for a in *.[ch]; do echo $$a ; splint $(CFLAGS) +posixstrictlib $$a > $$a.lint; done
gindent:
	gindent *.[ch]


