Summary: GTK+/GNOME MUD client
Name: bmud
Version: 0.6
Release: 1
Copyright: GPL
Vendor: Erik Greenwald
Url: http://math.smsu.edu/~br0ke/bmud.html
Group: Applications/Text
Source0: http://math.smsu.edu/~br0ke/files/bmud-0.6.tar.gz
Buildroot: /var/tmp/bmud

%description

%prep
%setup -q
%build
CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=/usr

make

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install

%clean
rm -rf $RPM_BUILD_ROOT

%files
/usr/*

%changelog

