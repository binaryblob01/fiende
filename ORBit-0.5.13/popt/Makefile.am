# Makefile for popt library.

AUTOMAKE_OPTIONS = 1.4 foreign

EXTRA_DIST = CHANGES autogen.sh $(noinst_MANS) popt.spec \
	testit.sh po/Makefile.in.in po/POTFILES.in po/popt.pot popt.ps \
	po/cs.po po/ro.po po/uk.po po/zh_CN.GB2312.po po/gl.po po/sk.po po/wa.po

SUBDIRS = intl po

INCLUDES = -I$(top_srcdir)

noinst_PROGRAMS = test1 test2
test1_SOURCES = test1.c
test1_LDFLAGS = -all-static
test1_LDADD = $(noinst_LTLIBRARIES)
test2_SOURCES = test2.c
test2_LDFLAGS = -all-static
test2_LDADD = $(noinst_LTLIBRARIES)

noinst_SCRIPTS = testit.sh

noinst_HEADERS = findme.h poptint.h system.h popt.h
noinst_LTLIBRARIES = libpopt.la
libpopt_la_SOURCES = popt.c findme.c poptparse.c poptconfig.c popthelp.c

noinst_MANS = popt.3

.PHONY: lclint
lclint:
	lclint ${DEFS} ${INCLUDES} ${libpopt_la_SOURCES}

CVSTAG = $(PACKAGE)-$(subst .,_,$(VERSION))

.PHONY: archive
archive: 
	@echo "This is $(PACKAGE)-$(VERSION)."
	@sleep 5
	@cvs -Q tag -F $(CVSTAG) .
	@rm -rf /tmp/$(PACKAGE)-$(VERSION) /tmp/$(PACKAGE)
	@cd /tmp; cvs -Q -d $(CVSROOT) export -r$(CVSTAG) $(PACKAGE) || :
	@mv /tmp/$(PACKAGE) /tmp/$(PACKAGE)-$(VERSION)
	@cd /tmp/$(PACKAGE)-$(VERSION); ./autogen.sh ; make depend; make distclean
	@cd /tmp/$(PACKAGE)-$(VERSION); ./autogen.sh --noconfigure
	@cd /tmp; tar czSpf $(PACKAGE)-$(VERSION).tar.gz $(PACKAGE)-$(VERSION)
	@rm -rf /tmp/$(PACKAGE)-$(VERSION)
	@cp /tmp/$(PACKAGE)-$(VERSION).tar.gz .
	@rm -f /tmp/$(PACKAGE)-$(VERSION).tar.gz 
	@echo " "
	@echo "The final archive is ./$(PACKAGE)-$(VERSION).tar.gz."
