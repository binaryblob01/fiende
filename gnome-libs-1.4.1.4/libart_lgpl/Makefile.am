SUBDIRS = doc

noinst_PROGRAMS = testart testuta gen_art_config

bin_SCRIPTS = \
	libart-config

BUILT_SOURCES = art_config.h

art_config.h:	gen_art_config
	./gen_art_config > art_config.h

man_MANS = libart-config.1

EXTRA_DIST = 			\
	libart.m4		\
	libartConf.sh.in	\
	libart-config.in	\
	$(man_MANS)

lib_LTLIBRARIES = libart_lgpl.la

libart_lgpl_la_SOURCES = \
	art_affine.c \
	art_alphagamma.c \
	art_bpath.c \
	art_gray_svp.c \
	art_misc.c \
	art_pixbuf.c \
	art_rect.c \
	art_rect_svp.c \
	art_rect_uta.c \
	art_rgb.c \
	art_rgb_affine.c \
	art_rgb_affine_private.c \
	art_rgb_affine_private.h \
	art_rgb_bitmap_affine.c \
	art_rgb_pixbuf_affine.c \
	art_rgb_rgba_affine.c \
	art_rgb_svp.c \
	art_svp.c \
	art_svp_ops.c \
	art_svp_point.c \
	art_svp_render_aa.c \
	art_svp_vpath.c \
	art_svp_vpath_stroke.c \
	art_svp_wind.c \
	art_uta.c \
	art_uta_ops.c \
	art_uta_rect.c \
	art_uta_vpath.c \
	art_uta_svp.c \
	art_vpath.c \
	art_vpath_bpath.c \
	art_vpath_dash.c \
	art_vpath_svp.c	\
	libart-features.c

libart_lgpl_la_LDFLAGS = -version-info @LIBART_VERSION_INFO@
libart_lgpl_la_LIBADD = -lm

libart_lgplincdir = $(includedir)/gnome-1.0/libart_lgpl
libart_lgplinc_HEADERS = \
	art_affine.h \
	art_alphagamma.h \
	art_bpath.h \
	art_config.h \
	art_filterlevel.h \
	art_gray_svp.h \
	art_misc.h \
	art_pathcode.h \
	art_pixbuf.h \
	art_point.h \
	art_rect.h \
	art_rect_svp.h \
	art_rect_uta.h \
	art_rgb.h \
	art_rgb_affine.h \
	art_rgb_bitmap_affine.h \
	art_rgb_pixbuf_affine.h \
	art_rgb_rgba_affine.h \
	art_rgb_svp.h \
	art_svp.h \
	art_svp_ops.h \
	art_svp_point.h \
	art_svp_render_aa.h \
	art_svp_vpath.h \
	art_svp_vpath_stroke.h \
	art_svp_wind.h \
	art_uta.h \
	art_uta_ops.h \
	art_uta_rect.h \
	art_uta_vpath.h \
	art_uta_svp.h \
	art_vpath.h \
	art_vpath_bpath.h \
	art_vpath_dash.h \
	art_vpath_svp.h	\
	libart.h	\
	libart-features.h

INCLUDES = -I$(top_srcdir) -I$(top_builddir) -DLIBART_COMPILATION

DEPS = $(top_builddir)/libart_lgpl.la
LDADDS = $(top_builddir)/libart_lgpl.la

testart_SOURCES=testart.c
testart_LDFLAGS =
testart_DEPENDENCIES = $(DEPS)
testart_LDADD = $(LDADDS) -lm

testuta_SOURCES=testuta.c
testuta_LDFLAGS =
testuta_DEPENDENCIES = $(DEPS)
testuta_LDADD = $(LDADDS) -lm

tests:	testart testuta

m4datadir = $(datadir)/aclocal
m4data_DATA = libart.m4

## Put `exec' in the name because this should be installed by
## `install-exec', not `install-data'.

confexecdir=$(libdir)
confexec_DATA = libartConf.sh

## We create libartConf.sh here and not from configure because we want
## to get the paths expanded correctly.  Macros like srcdir are given
## the value NONE in configure if the user doesn't specify them (this
## is an autoconf feature, not a bug).

libartConf.sh: libartConf.sh.in Makefile
## Use sed and then mv to avoid problems if the user interrupts.
	sed -e 's?\@LIBART_LIBDIR\@?$(LIBART_LIBDIR)?g' \
	    -e 's?\@LIBART_INCLUDEDIR\@?$(LIBART_INCLUDEDIR)?g' \
	    -e 's?\@LIBART_LIBS\@?$(LIBART_LIBS)?g' \
	      < $(srcdir)/libartConf.sh.in > libartConf.tmp \
	  && mv libartConf.tmp libartConf.sh
