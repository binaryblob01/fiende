SUBDIRS = private

# this is to omit -I. and -I$(srcdir) from the DEFS variable.
DEFS = -DHAVE_CONFIG_H -I../.. -DGTKMM_IMPLEMENTATION

INCLUDES = -DG_LOG_DOMAIN=\"Gtk--\" \
           -I$(srcdir)/.. -I.. \
           -I$(top_srcdir)/gdk-- \
           @GTK_CFLAGS@ \
           @SIGC_CFLAGS@ 


gtkmmhwithdirs = $(GEN_H_FILES:.gen_h=.h)
gtkmmla = $(patsubst %.gen_h,%.lo,$(GEN_H_FILES))
gtkmmh = $(gtkmmhwithdirs)
gtkmmc = wrap.cc $(gtkmmh:.h=.cc) 

# Not working with all only... trying all-local :(
all-local: Makefile $(DATA) $(LTLIBRARIES)

# We don't wish to treat our sources as built.
#BUILT_SOURCES = $(gtkmmh) $(gtkmmc)

# we will not include headers and sources to distribution as they're
# generated by our makefiles. (this one is needed because make dist
# doesnt work with our generated files correctly - gives error because
# it tries to get these from $(srcdir) and not $(builddir))
DISTFILES = $(DIST_COMMON) $(TEXINFOS) $(EXTRA_DIST)
EXTRA_DIST = marshal.h marshal.cc base.h base.cc gtk--.cc proxy.cc imageloader.h

# This installs headers needed by gtk--
# use DATA rather than HEADERS, so that the install comes from the
# obj directory rather than the src directory.
#gtkmmdir=$(prefix)/include/gtk--
#gtkmm_DATA = $(gtkmmh) proxy.h
# satisfy automake 1.4
# EXTRA_DATA = calendar.h invisible.h \
# 	accelgroup.h accellabel.h \
# 	ctree.h fontselection.h itemfactory.h \
# 	progress.h packer.h \
# 	layout.h plug.h socket.h

EXTRA_HEADERS = 
includedir = $(prefix)/include/gtk--
include_HEADERS = $(gtkmmh) marshal.h proxy.h base.h imageloader.h


# This builds the library
libgtkmm_la_SOURCES = $(gtkmmc) gtk--.cc proxy.cc base.cc marshal.cc
libgtkmm_la_LDFLAGS = -version-info $(LIBGTKMM_SO_VERSION) -release $(GTKMM_RELEASE) -no-undefined
libgtkmm_la_INCLUDES = $(GTK_CFLAGS)
libgtkmm_la_LIBADD = $(GTK_LIBS) $(gtkmmla)
#libgtkmm_la_DEPENDENCIES = $(gtkmmla) 
lib_LTLIBRARIES = libgtkmm.la

# we need to omit dependencies to files that might not exists on all
# builds. (guess it doesnt work if we try to take these from
# some variables.. :)
#OMIT_DEPENDENCIES = 
