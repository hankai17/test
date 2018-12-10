Summary:   The icdns is a advanced, high performance resolver
Name:      icdns
Version:   4.0
Release:   6.0.el7
License:   GPLv2
Group:     System exec file
Source:    icdns.tar.gz
Packager:  xiaoliang.hao<xiaoliang.hao@intcache.com>
#Requires:  pthread
BuildRoot: %{_topdir}/BUILDROOT
Autoreq:   no
 
%description
This package is capture http,dns pkt and redirect to cdn server
 
%prep
mkdir -p %{buildroot}/usr/sbin
mkdir -p %{buildroot}/usr/bin
mkdir -p %{buildroot}/etc/icdns
mkdir -p %{buildroot}/etc/init.d
mkdir -p %{buildroot}/usr/local/boost/lib
mkdir -p %{buildroot}/usr/lib64
echo package "%{name}-%{version}-%{release}" begin installing 

%setup -c

%install
rm -fr %{buildroot}/*
mkdir -p %{buildroot}/usr/sbin
mkdir -p %{buildroot}/usr/bin
mkdir -p %{buildroot}/etc/icdns
mkdir -p %{buildroot}/etc/init.d
mkdir -p %{buildroot}/usr/local/boost/lib
mkdir -p %{buildroot}/usr/lib64
install -m 755  icdns           %{buildroot}/usr/sbin/
install -m 755  rec_control     %{buildroot}/usr/bin/
install -m 755  icdns.initd     %{buildroot}/etc/init.d/icdns
install -m 666  recursor.conf   %{buildroot}/etc/icdns/
install -m 755  sdmail.pl       %{buildroot}/etc/icdns/
install -m 666  domain-white-list       %{buildroot}/etc/icdns/
install -m 666  domain-black-list       %{buildroot}/etc/icdns/
if [ `uname -r | awk -F'el' '{print $2}' | cut -d. -f1` -lt 7 ];then
install -m 755  libboost_context.so.1.63.0 %{buildroot}/usr/local/boost/lib/
install -m 755  libboost_system.so.1.63.0  %{buildroot}/usr/local/boost/lib/
install -m 755  libboost_thread-mt.so.5    %{buildroot}/usr/local/boost/lib/
else
install -m 755  libboost_context.so.1.63.0.el7      %{buildroot}/usr/local/boost/lib/libboost_context.so.1.63.0
install -m 755  libboost_system.so.1.63.0.el7       %{buildroot}/usr/local/boost/lib/libboost_system.so.1.63.0
install -m 755  libboost_thread-mt.so.1.53.0.el7    %{buildroot}/usr/local/boost/lib/libboost_thread-mt.so.1.53.0
install -m 755  libboost_thread.so.1.63.0.el7    %{buildroot}/usr/local/boost/lib/libboost_thread.so.1.63.0
install -m 755  libboost_chrono.so.1.63.0.el7    %{buildroot}/usr/local/boost/lib/libboost_chrono.so.1.63.0
fi
install -m 755  libstdc++.so.6.0.22.gz     %{buildroot}/usr/lib64/

%post
getent group icdns > /dev/null || groupadd -r icdns
getent passwd icdns > /dev/null || useradd -r -g icdns -d / -s /sbin/nologin -c "PowerDNS Recursor user" icdns
/sbin/chkconfig --add icdns
if [ `uname -r | awk -F'el' '{print $2}' | cut -d. -f1` -lt 7 ];then
	cd /usr/lib64 && gzip -d libstdc++.so.6.0.22.gz && ln -sf libstdc++.so.6.0.22 libstdc++.so.6
fi
touch /etc/icdns/ip-white-list
mkdir -p /var/run/icdns/
#test -e /etc/icdns/sdmail.pl.rpmnew && cp -f /etc/icrdns/sdmail.pl.rpmnew /etc/icdns/sdmail.pl
echo package "%{name}-%{version}-%{release}" installed successfully

%files
%defattr(-,root,root)
/usr/sbin/icdns
/usr/bin/rec_control
/etc/init.d/icdns
/etc/icdns/sdmail.pl
/usr/local/boost/lib
/usr/lib64/libstdc++.so.6.0.22.gz
%config(noreplace) /etc/icdns/recursor.conf
%config(noreplace) /etc/icdns/domain-white-list
%config(noreplace) /etc/icdns/domain-black-list

%changelog
* Tue Jun 22 2017 version 4.0.3.1 
- add ics server probe

%preun
/etc/init.d/icdns stop
/sbin/chkconfig --del icdns
rm -fr /usr/src/redhat/BUILDROOT/*

%postun
rm -fr /usr/sbin/icdns
rm -fr /usr/bin/rec_control
rm -fr /etc/icdns
echo package "%{name}-%{version}-%{release}" uninstalled successfully
