## Process this file with automake to produce Makefile.in
# The name of the module.
DOC_MODULE=gnorba

# The top-level SGML file.
DOC_MAIN_SGML_FILE=gnorba.sgml

# The directory containing the source code (if it contains documentation).
DOC_SOURCE_DIR=$(GNOME_LIBS_DIR)/libgnorba

HTML_DIR=$(datadir)/gnome/html

TARGET_DIR=$(HTML_DIR)/$(DOC_MODULE)

tmpl_sources =				\
	tmpl/gnorba-unused.sgml		\
	tmpl/goad.sgml			\
	tmpl/gnorba.sgml

gnorba_docdir = $(HTML_DIR)
gnorba_doc_DATA = 			\
	gnorba-decl.txt			\
	gnorba-sections.txt

EXTRA_DIST = $(gnorba_doc_DATA) gnorba-docs.sgml

all-local: html/book1.html

if ENABLE_GTK_DOC
html/book1.html: sgml/gnorba-doc.bottom
	$(MAKE) html
else
gnorba.html:
html/book1.html:
endif

sgml/gnorba-doc.bottom: $(tmpl_sources) gnorba-docs.sgml
	$(MAKE) sgml	

scan:
	cd $(srcdir) \
	&& gtkdoc-scan --module=$(DOC_MODULE) \
		       --source-dir=$(DOC_SOURCE_DIR)

templates: scan
	cd $(srcdir) && gtkdoc-mktmpl --module=$(DOC_MODULE)

sgml:
	cd $(srcdir) \
	&& gtkdoc-mkdb --module=$(DOC_MODULE) --source-dir=$(DOC_SOURCE_DIR)

html:
	-test -d $(srcdir)/html || mkdir $(srcdir)/html
	-cd $(srcdir)/html && gtkdoc-mkhtml $(DOC_MODULE) ../gnorba-docs.sgml
	-cd $(srcdir)/html && perl -pi -e 's,libgnorba\.html,index.html,gi' *.html

clean-local:
	rm -f *~ *.bak *.hierarchy *.signals *-unused.txt

maintainer-clean-local: clean
	cd $(srcdir) && rm -rf sgml html $(DOC_MODULE)-decl-list.txt $(DOC_MODULE)-decl.txt 

install-data-local: 
	$(mkinstalldirs) $(DESTDIR)$(TARGET_DIR)
	(installfiles=`echo $(srcdir)/html/*.html`; \
	if test "$$installfiles" = '$(srcdir)/html/*.html'; \
	then echo '-- Nothing to install' ; \
	else \
	  for i in $$installfiles; do \
	    echo '-- Installing '$$i ; \
	    $(INSTALL_DATA) $$i $(DESTDIR)$(TARGET_DIR); \
	  done; \
	  echo '-- Installing $(srcdir)/html/index.sgml' ; \
	  $(INSTALL_DATA) $(srcdir)/html/index.sgml $(DESTDIR)$(TARGET_DIR); \
	  echo '-- Fixing Crossreferences' ; \
	  gtkdoc-fixxref --module=$(DOC_MODULE) --html-dir=$(HTML_DIR) || true; \
	fi)

dist-hook:
	-mkdir $(distdir)/html
	-mkdir $(distdir)/sgml
	-mkdir $(distdir)/tmpl
	-cp $(srcdir)/html/*.html $(srcdir)/html/*.css $(distdir)/html
	-cp $(srcdir)/tmpl/*.sgml $(distdir)/tmpl
	-cp $(srcdir)/sgml/*.sgml $(distdir)/sgml
	-cp $(srcdir)/sgml/*.bottom $(srcdir)/sgml/*.top $(distdir)/sgml

.PHONY : html sgml templates scan
