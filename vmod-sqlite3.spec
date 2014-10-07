Name: vmod-sqlite3
Version: 0.1
Release: 1%{?dist}
Summary: SQLite3 VMOD for Varnish

Group: System Environment/Daemons
License: BSD
URL: https://github.com/fgsch/libvmod-sqlite3
Source0: https://github.com/fgsch/libvmod-sqlite3/libvmod-sqlite3-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires: pkgconfig(varnishapi)
BuildRequires: python-docutils
BuildRequires: varnish >= 4.0
BuildRequires: varnish < 4.1
Requires: varnish >= 4.0
Requires: varnish < 4.1

%description
SQLite3 VMOD for Varnish

%prep
%setup -q -n libvmod-sqlite3-%{version}

%build
%configure --docdir='%_docdir/%{name}-%{version}'
make

%check
make check

%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%{_libdir}/varnish*/vmods/*
%doc LICENSE
%{_mandir}/man3/*

%changelog
* Wed Mar 25 2015 Federico G. Schwindt <fgsch@lodoss.net> - 0.1-1
- Initial version.
