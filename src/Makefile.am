# $Id: Makefile.am,v 1.4 2004/01/04 15:23:53 erik Exp $

bin_PROGRAMS = bmud 

bmud_SOURCES = \
	alias.c\
	alias_utils.c\
	color.c config.c\
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
	config.h\
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


