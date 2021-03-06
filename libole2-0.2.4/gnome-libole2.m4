AC_DEFUN([GNOME_INIT_LIBOLE2],
[
	AC_MSG_CHECKING(for an libole2Conf.sh)
	my_gnome_libdir=`$GNOME_CONFIG --libdir`
	if test -f $my_gnome_libdir/libole2Conf.sh; then
	    . $my_gnome_libdir/libole2Conf.sh
	    AC_MSG_RESULT(found $my_gnome_libdir)
	    ac_cv_have_libole2=yes
	else
	    AC_MSG_RESULT(not found)
	    AC_MSG_WARN(Could not find the libole2Conf.sh file that is generated by libole2 install)
	    ac_cv_have_libole2=no
	fi
	
	dnl Add a conditional on whether or not we have libole2
	AM_CONDITIONAL(HAVE_LIBOLE2, test x$ac_cv_have_libole2 = xyes)
	HAVE_LIBOLE2=$ac_cv_have_libole2

	AC_SUBST(LIBOLE2_INCLUDEDIR)
	AC_SUBST(LIBOLE2_LIBDIR)
	AC_SUBST(LIBOLE2_LIBS)
])
