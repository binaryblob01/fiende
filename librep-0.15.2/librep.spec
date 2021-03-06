
Name: librep
Version: 0.15.2
Release: 1
Summary: An embeddable LISP environment
Copyright: GPL
Group: Development/Languages
Source: http://download.sourceforge.net/librep/librep-%{version}.tar.gz
URL: http://librep.sourceforge.net/
Packager: John Harper <john@dcs.warwick.ac.uk>
Buildroot: /var/tmp/%{name}-root
BuildPrereq: gmp-devel gdbm-devel readline-devel texinfo
PreReq: /sbin/install-info
Requires: gdbm >= 1.8

%description
This is a lightweight Lisp environment for UNIX. It contains a Lisp
interpreter, byte-code compiler and virtual machine. Applications may
use the Lisp interpreter as an extension language, or it may be used
for standalone scripts.

Originally inspired by Emacs Lisp, the language dialect combines many
of the elisp features while trying to remove some of the main
deficiencies, with features from Common Lisp and Scheme.

%package devel
Summary: librep include files and link libraries
Group: Development/Languages
Requires: %{name} = 0.15.2

%description devel
Link libraries and C header files for librep development.

%prep
%setup -q

%build

LC_ALL=""
LINGUAS=""
LANG=""
export LC_ALL LINGUAS LANG

%configure --with-readline --with-extra-cflags=-fstrength-reduce
make host_type=%{_target_platform}

%install
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf $RPM_BUILD_ROOT
make install \
    host_type=%{_target_platform} \
    DESTDIR=$RPM_BUILD_ROOT \
    aclocaldir=%{_datadir}/aclocal \
    infodir=%{_infodir}
gzip -9nf $RPM_BUILD_ROOT%{_infodir}/librep*

%post
/sbin/ldconfig
/sbin/install-info %{_infodir}/librep.info.gz %{_infodir}/dir

%preun
if [ "$1" = 0 ]; then
    /sbin/install-info --delete %{_infodir}/librep.info.gz %{_infodir}/dir
fi

%postun -p /sbin/ldconfig

%clean
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc NEWS README THANKS TODO BUGS
%{_bindir}/rep
%{_bindir}/rep-remote
%{_libdir}/librep.so.*
%{_datadir}/rep/%{version}
%{_libexecdir}/rep/%{version}/%{_target_platform}
%{_infodir}/librep*
%{_datadir}/emacs/site-lisp/rep-*

%files devel
%defattr(-,root,root)
%{_bindir}/rep-config
%{_bindir}/rep-xgettext
%{_bindir}/repdoc
%{_libdir}/librep.so
%{_includedir}/rep.h
%{_includedir}/rep_*.h
%{_libexecdir}/rep/%{_target_platform}/libtool
%{_libexecdir}/rep/%{_target_platform}/rules.mk
%{_libexecdir}/rep/%{_target_platform}/install-aliases
%{_libexecdir}/rep/%{_target_platform}/rep_config.h
%{_datadir}/aclocal/rep.m4

%changelog
* Tue Jun 13 2000 John Harper <john@dcs.warwick.ac.uk>
- use better macros

* Wed Nov 10 1999 Michael K. Johnson <johnsonm@redhat.com>
- post{,un} use -p

* Mon Sep 13 1999 Aron Griffis <agriffis@bigfoot.com>
- 0.5 spec file update: added buildroot
