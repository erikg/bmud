# $Id: Makefile.am,v 1.9 2005/10/04 20:21:10 erik Exp $

SUBDIRS = data doc src #test

.PHONY: rpm deb packages lint indent
rpm: dist
	cp $(PACKAGE)-$(VERSION).tar.gz ${HOME}/rpm/SOURCES/
	rpmbuild -ba $(PACKAGE).spec
	cp ${HOME}/rpm/RPMS/i386/$(PACKAGE)-$(VERSION)-*.i386.rpm .
	cp ${HOME}/rpm/SRPMS/$(PACKAGE)-$(VERSION)-*.src.rpm .

deb: 
	debian/rules
	debuild
	debc
	debi
	release

packages: rpm

lint:
	make -C src lint
indent:
	make -C src indent

test:
	make -C test test

docs:
	doxygen doc/doxygen.conf
	make -C doc/doxygen/latex 
	dvips -f doc/doxygen/latex/refman.dvi > refman.ps
	ps2pdf refman.ps
