Name:           csvtools
Version:        0.1
Release:        1%{?dist}
Summary:        CSV fast processing tools

License:        MIT
URL:            https://github.com/DavyLandman/csvtools
BuildRequires:	gcc, pcre-devel
%description
%build
echo toto
cd %{_topdir}
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/local/bin
%make_install


%files
/usr/local/bin/csvawk
/usr/local/bin/csvcut
/usr/local/bin/csvgrep
/usr/local/bin/csvpipe
/usr/local/bin/csvunpipe


%changelog
