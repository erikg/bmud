# $Id: Makefile.am,v 1.3 2004/01/04 15:42:36 erik Exp $

EXTRA_DIST = bmud.desktop bmud.html bmud.png mudservers topic.dat

install:
	mkdir -p ${datadir}/gnome/apps/Games
	mkdir -p ${datadir}/gnome/help/bmud/C
	mkdir -p ${datadir}/pixmaps
	install -m 644 bmud.desktop ${datadir}/gnome/apps/Games/bmud.desktop
	install -m 644 topic.dat ${datadir}/gnome/help/bmud/C/topic.dat
	install -m 644 bmud.html ${datadir}/gnome/help/bmud/C/bmud.html
	install -m 644 bmud.png ${datadir}/pixmaps/bmud.png

uninstall:
	rm -f ${datadir}/gnome/apps/Games/bmud.desktop
	rm -rf ${datadir}/gnome/help/bmud
	rm -f ${datadir}/pixmaps/bmud.png
