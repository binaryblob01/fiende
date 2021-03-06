#
# Note that this is NOT a relocatable package
# $Id: medusa.spec.in,v 1.42 2001/04/07 00:47:04 rebecka Exp $
#
%define name	 medusa
%define ver      0.5.1
%define RELEASE  1	 
%define rel      %{?CUSTOM_RELEASE} %{!?CUSTOM_RELEASE:%RELEASE}
%define prefix   /usr/
%define sysconfdir /etc
%define localstatedir /var

Summary: Medusa, the search and indexing package for use with Eazel's Nautilus.
Name: %name
Version: %ver
Release: %rel
Copyright: LGPL
Group: System Environment/Base
Source: ftp://ftp.gnome.org/pub/GNOME/unstable/sources/medusa/medusa-%{ver}.tar.gz
BuildRoot: /var/tmp/medusa
URL: http://www.gnome.org
Prereq: /sbin/install-info
Prefix: %{prefix}
Docdir: %{prefix}/doc
Requires: glib >= 1.2.0
Requires: gnome-vfs >= 0.1

%description
Medusa, the GNOME search/indexing package.

%package devel
Summary:        Libraries and include files for developing nautilus components
Group:          Development/Libraries
Requires:       %name = %{PACKAGE_VERSION}
Obsoletes:      %{name}-devel

%description devel
This package provides the necessary development libraries and include 
files to allow you to develop medusa components.

%changelog
* Thu Mar 29 2001  Rebecca Schulman <rebecka@eazel.com>
- Remove /com/medusa stuff.

* Thu Mar 01 2001  Rebecca Schulman <rebecka@eazel.com>
- Add sharedstatedir info to the spec file

- Intermediate changes here, not recorded -

* Sun Jun 11 2000  Eskil Heyn Olsen <deity@eazel.com>
- Created the .spec file

%prep
%setup

%build
# Needed for snapshot releases.
if [ ! -f configure ]; then
	CFLAGS="$RPM_OPT_FLAGS" ./autogen.sh --enable-more-warnings --prefix=%prefix --sysconfdir=/etc --localstatedir=/var 
else
	CFLAGS="$RPM_OPT_FLAGS" ./configure --enable-more-warnings --prefix=%prefix --sysconfdir=/etc --localstatedir=/var 
fi
make -k check

%install
rm -rf $RPM_BUILD_ROOT
make -k prefix=$RPM_BUILD_ROOT%{prefix} sysconfdir=$RPM_BUILD_ROOT%{sysconfdir} localstatedir=$RPM_BUILD_ROOT%{localstatedir} sharedstatedir=$RPM_BUILD_ROOT%{sharedstatedir} install

%clean
rm -rf $RPM_BUILD_ROOT

%post
/sbin/ldconfig

INITDIR=""
for i in "/etc" "/etc/rc.d"
do
	if test -d "$i/init.d" && test -d "$i/rc0.d" && test -d "$i/rc2.d"
	then
		INITDIR="$i"
	fi
done

case "$INITDIR" in
	'')
		echo "Can't find init script directory" >&2
		exit 1
	;;
esac

rm -f "$INITDIR/rc2.d/S99medusa"
rm -f "$INITDIR/rc3.d/S99medusa"
rm -f "$INITDIR/rc5.d/S99medusa"
rm -f "$INITDIR/rc0.d/K00medusa"

cp "%{prefix}/tmp/medusa-init" "$INITDIR/init.d/medusa"
ln -s ../init.d/medusa "$INITDIR/rc5.d/S99medusa"
ln -s ../init.d/medusa "$INITDIR/rc3.d/S99medusa"
ln -s ../init.d/medusa "$INITDIR/rc2.d/S99medusa"
ln -s ../init.d/medusa "$INITDIR/rc0.d/K00medusa"

%postun
/sbin/ldconfig

INITDIR=""
for i in "/etc" "/etc/rc.d"
do
	if test -d "$i/init.d" && test -d "$i/rc0.d" && test -d "$i/rc2.d"
	then
		INITDIR="$i"
	fi
done

case "$INITDIR" in
	'')
		echo "Can't find init script directory" >&2
		exit 1
	;;
esac

rm -f "$INITDIR/init.d/medusa"
rm -f "$INITDIR/rc5.d/S99medusa"
rm -f "$INITDIR/rc3.d/S99medusa"
rm -f "$INITDIR/rc2.d/S99medusa"
rm -f "$INITDIR/rc0.d/K00medusa"

%files
%defattr (0755, bin, bin)
%config %{sysconfdir}/cron.daily/medusa.cron
%config %{prefix}/tmp/medusa-init
%config /etc/profile.d/medusa-idled.sh
%config /etc/profile.d/medusa-idled.csh



%defattr(-, bin, bin)
%config %{sysconfdir}/vfs/modules/*.conf
%doc doc/how_to_use_msearch doc/search_uri_rfc
%{prefix}/bin/medusa-idled
%{prefix}/bin/medusa-config
%{prefix}/bin/medusa-indexd
%{prefix}/bin/medusa-searchd
%{prefix}/bin/msearch
%{prefix}/lib/*.0
%{prefix}/lib/*.so
%{prefix}/lib/vfs/modules/*.so
%{prefix}/sbin/medusa-test-indexing-status
%config %{sysconfdir}/medusa/medusa.conf
%config %{sysconfdir}/medusa/file-index-stoplist
%config %{sysconfdir}/medusa/mount-type-stoplist
%dir %{localstatedir}/medusa

%doc AUTHORS COPYING ChangeLog NEWS README
%{prefix}/man/man1/*
%{prefix}/man/man5/*
%{prefix}/man/man7/*
%{prefix}/man/man8/*

%files devel
%defattr(-, bin, bin)
%{prefix}/include/libmedusa/*.h
%{prefix}/lib/*.la
%{prefix}/lib/vfs/modules/*.la
