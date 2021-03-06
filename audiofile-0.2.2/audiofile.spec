%define  ver     0.2.2
%define  RELEASE 1
%define  rel     %{?CUSTOM_RELEASE} %{!?CUSTOM_RELEASE:%RELEASE}
%define  prefix  /usr

Summary: A library to handle various audio file formats.
Name: audiofile
Version: %ver
Release: %rel
Copyright: LGPL
Group: Libraries/Sound
Source: ftp://ftp.gnome.org/pub/GNOME/sources/audiofile/audiofile-%{PACKAGE_VERSION}.tar.gz
URL: http://www.68k.org/~michael/audiofile/
BuildRoot:/var/tmp/audiofile-%{PACKAGE_VERSION}-root
Docdir: %{prefix}/doc
Obsoletes: libaudiofile

%description
The Audio File Library provides an elegant API for accessing a variety
of audio file formats, such as AIFF/AIFF-C, WAVE, and NeXT/Sun
.snd/.au, in a manner independent of file and data formats.

%package devel
Summary: Library, headers, etc. to develop with the Audio File Library.
Group: Libraries

%description devel
Library, header files, etc. for developing applications with the Audio
File Library.

%prep
%setup

%build
CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%prefix
make $MAKE_FLAGS

%install

rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT

#
# makefile is broken, sets exec_prefix explicitely.
#
make exec_prefix=$RPM_BUILD_ROOT/%{prefix} prefix=$RPM_BUILD_ROOT/%{prefix} install 

%clean
rm -rf $RPM_BUILD_ROOT

%changelog

* Fri Nov 20 1998 Michael Fulbright <drmike@redhat.com>
- First try at a spec file

%files
%defattr(-, root, root)
%doc COPYING TODO README ChangeLog docs
%{prefix}/bin/*
%{prefix}/lib/lib*.so.*

%files devel
%defattr(-, root, root)
%{prefix}/lib/lib*.so
%{prefix}/lib/*.a
%{prefix}/include/*
%{prefix}/share/aclocal/*
