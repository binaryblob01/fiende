# Note that this is NOT a relocatable package
%define ver      1.0.13
%define  RELEASE 1
%define  rel     %{?CUSTOM_RELEASE} %{!?CUSTOM_RELEASE:%RELEASE}
%define prefix   /usr

Summary: GNOME System Monitor
Name: gtop
Version: %ver
Release: %rel
Copyright: GPL
Group: Applications/System
Source: ftp://ftp.gnome.org/pub/GNOME/sources/gtop/gtop-%{ver}.tar.gz
BuildRoot: /var/tmp/gtop-root
Obsoletes: gnome
Packager: Marc Ewing <marc@redhat.com>
URL: http://www.gnome.org
Docdir: %{prefix}/doc

%description
GNOME System Monitor.

GNOME is the GNU Network Object Model Environment.  That's a fancy
name but really GNOME is a nice GUI desktop environment.  It makes
using your computer easy, powerful, and easy to configure.

%changelog

* Sun Aug 23 1998 Martin Baulig <martin@home-of-linux.org>

- Make GTop its own top-level module.

%prep
%setup

%build
%ifarch alpha
	MYARCH_FLAGS="--host=alpha-redhat-linux"
%endif

if [ ! -f configure ]; then
	CFLAGS="$RPM_OPT_FLAGS" ./autogen.sh $MYARCH_FLAGS --prefix=%prefix 
else
	CFLAGS="$RPM_OPT_FLAGS" ./configure $MYARCH_FLAGS --prefix=%prefix 
fi

if [ "$SMP" != "" ]; then
	make -k -j$SMP check "MAKE=make -k -j$SMP check"
else
	make -k check
fi

%install
rm -rf $RPM_BUILD_ROOT

make -k prefix=$RPM_BUILD_ROOT%{prefix} install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)

%doc AUTHORS BUG-REPORTING COPYING NEWS README
%{prefix}/bin/*
%{prefix}/share/gnome/apps/System/gtop.desktop
%{prefix}/share/gnome/help/gtop/C/*.html
%{prefix}/share/gnome/help/gtop/C/*.dat
%{prefix}/share/locale/*/LC_MESSAGES/*.mo
%{prefix}/share/pixmaps/gnome-gtop.png
