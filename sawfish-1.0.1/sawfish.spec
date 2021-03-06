Summary: An extensible window manager for the X Window System.
Name: sawfish
Version: 1.0.1
Release: 1
Requires: librep >= 0.14, rep-gtk >= 0.14, rep-gtk-gnome
Copyright: GPL
Group: User Interface/Desktops
Source: http://download.sourceforge.net/sawfish/sawfish-%{version}.tar.gz
URL: http://sawmill.sourceforge.net/
Packager: John Harper <john@dcs.warwick.ac.uk>
Buildroot: /var/tmp/%{name}-root
BuildPrereq: rep-gtk-libglade control-center-devel gmp-devel texinfo
PreReq: /sbin/install-info
Provides: windowmanager
Obsoletes: sawmill sawmill-gnome sawfish-gnome

%description
Sawfish is an extensible window manager which uses a Lisp-based
scripting language.  All window decorations are configurable and the
basic idea is to have as much user-interface policy as possible
controlled through the Lisp language.  Configuration can be
accomplished by writing Lisp code in a personal .sawfishrc file, or
using a GTK+ interface.  Sawfish is mostly GNOME compliant

%package themer
Summary: A GUI for creating sawfish window manager themes.
Group: User Interface/Desktops
Requires: %{name} = 1.0.1, rep-gtk-libglade
Obsoletes: sawmill-themer

%description themer
The sawfish-themer package contains an optional theme builder for the
sawfish window manager. sawfish-themer allows static window themes to
be created and edited in a graphical environment.

%prep
%setup -q

%build

LC_ALL=""
LINGUAS=""
LANG=""
export LC_ALL LINGUAS LANG

%configure
make host_type=%{_target_platform}

%install
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{_datadir}/gnome/wm-properties
mkdir -p $RPM_BUILD_ROOT%{_datadir}/control-center
mkdir -p $RPM_BUILD_ROOT%{_datadir}/gnome/apps/Settings
mkdir -p $RPM_BUILD_ROOT%{_bindir}
make install \
    DESTDIR=$RPM_BUILD_ROOT \
    GNOMEDIR=%{_prefix} \
    host_type=%{_target_platform} \
    infodir=%{_infodir}
gzip -9nf $RPM_BUILD_ROOT%{_infodir}/sawfish*

%post
/sbin/install-info %{_infodir}/sawfish.info.gz %{_infodir}/dir

%preun
if [ "$1" = 0 ]; then
    /sbin/install-info --delete %{_infodir}/sawfish.info.gz %{_infodir}/dir
fi

%clean
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc README NEWS FAQ THANKS BUGS TODO
%doc po/sawfish.pot lisp/sawfish/ui/WIDGETS lisp/sawfish/ui/WISHLIST
%{_bindir}/sawfish
%{_bindir}/sawfish-client
%{_bindir}/sawfish-ui
%{_bindir}/sawfish-capplet
%{_datadir}/sawfish/%{version}
%{_datadir}/locale
%{_datadir}/control-center/Sawfish
%{_datadir}/gnome/apps/Settings/Sawfish
%{_datadir}/gnome/wm-properties/Sawfish.desktop
%{_datadir}/pixmaps/sawfish*.png
%{_libexecdir}/sawfish/%{version}/%{_target_platform}
%{_libexecdir}/rep/%{_target_platform}/sawfish
%{_infodir}/sawfish*

%files themer
%defattr(-,root,root)
%{_bindir}/sawfish-themer
%{_datadir}/sawfish/%{version}/themer.glade

%changelog
* Tue Jun 12 2000 John Harper <john@dcs.warwick.ac.uk>
- merged differences from RH spec file

* Mon Apr 24 2000 John Harper <john@dcs.warwick.ac.uk>
- s/sawmill/sawfish/

* Fri Sep 17 1999 John Harper <john@dcs.warwick.ac.uk>
- don't patch the Makefile

* Tue Sep 14 1999 Aron Griffis <agriffis@bigfoot.com>
- 0.6 spec file update: added buildroot
