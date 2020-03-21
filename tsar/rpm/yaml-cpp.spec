Name: yaml-cpp
Version: 6.3
Release: 3
Summary: yaml-parser
URL: https://github.com/jbeder/yaml-cpp
Group: jbeder
License: MIT
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)                                                     
Source: yaml-cpp-%{version}.tar.gz

%description
yaml parser & deparser

%package devel 
Summary: yaml parse & deparse
Group: No
%description devel
devel package include header files and modules template for the development                                               

%prep
%setup -q

%install

rm -rf %{buildroot}
mkdir -p %{buildroot}/usr/local/lib/
mkdir -p %{buildroot}/usr/local/include/yaml-cpp/
mkdir -p %{buildroot}/usr/local/include/yaml-cpp/contrib/
mkdir -p %{buildroot}/usr/local/include/yaml-cpp/node/
mkdir -p %{buildroot}/usr/local/include/yaml-cpp/node/detail/

install -m 0755 modules/libyaml-cpp.so %{buildroot}/usr/local/lib/
install -m 0755 modules/libyaml-cpp.so.0.6 %{buildroot}/usr/local/lib/
install -m 0755 modules/libyaml-cpp.so.0.6.3 %{buildroot}/usr/local/lib/
install -p -D -m 0644 devel/yaml-cpp/anchor.h %{buildroot}/usr/local/include/yaml-cpp/
install -p -D -m 0644 devel/yaml-cpp/binary.h %{buildroot}/usr/local/include/yaml-cpp/
install -p -D -m 0644 devel/yaml-cpp/dll.h %{buildroot}/usr/local/include/yaml-cpp/
install -p -D -m 0644 devel/yaml-cpp/emitfromevents.h %{buildroot}/usr/local/include/yaml-cpp/
install -p -D -m 0644 devel/yaml-cpp/emitter.h %{buildroot}/usr/local/include/yaml-cpp/
install -p -D -m 0644 devel/yaml-cpp/emitterdef.h %{buildroot}/usr/local/include/yaml-cpp/
install -p -D -m 0644 devel/yaml-cpp/emittermanip.h %{buildroot}/usr/local/include/yaml-cpp/
install -p -D -m 0644 devel/yaml-cpp/emitterstyle.h %{buildroot}/usr/local/include/yaml-cpp/
install -p -D -m 0644 devel/yaml-cpp/eventhandler.h %{buildroot}/usr/local/include/yaml-cpp/
install -p -D -m 0644 devel/yaml-cpp/exceptions.h %{buildroot}/usr/local/include/yaml-cpp/
install -p -D -m 0644 devel/yaml-cpp/mark.h %{buildroot}/usr/local/include/yaml-cpp/
install -p -D -m 0644 devel/yaml-cpp/null.h %{buildroot}/usr/local/include/yaml-cpp/
install -p -D -m 0644 devel/yaml-cpp/ostream_wrapper.h %{buildroot}/usr/local/include/yaml-cpp/
install -p -D -m 0644 devel/yaml-cpp/parser.h %{buildroot}/usr/local/include/yaml-cpp/
install -p -D -m 0644 devel/yaml-cpp/stlemitter.h %{buildroot}/usr/local/include/yaml-cpp/
install -p -D -m 0644 devel/yaml-cpp/traits.h %{buildroot}/usr/local/include/yaml-cpp/
install -p -D -m 0644 devel/yaml-cpp/yaml.h %{buildroot}/usr/local/include/yaml-cpp/

install -p -D -m 0644 devel/yaml-cpp/contrib/anchordict.h %{buildroot}/usr/local/include/yaml-cpp/contrib/
install -p -D -m 0644 devel/yaml-cpp/contrib/graphbuilder.h %{buildroot}/usr/local/include/yaml-cpp/contrib/
install -p -D -m 0644 devel/yaml-cpp/node/convert.h %{buildroot}/usr/local/include/yaml-cpp/node/

install -p -D -m 0644 devel/yaml-cpp/node/emit.h %{buildroot}/usr/local/include/yaml-cpp/node/
install -p -D -m 0644 devel/yaml-cpp/node/impl.h %{buildroot}/usr/local/include/yaml-cpp/node/
install -p -D -m 0644 devel/yaml-cpp/node/iterator.h %{buildroot}/usr/local/include/yaml-cpp/node/
install -p -D -m 0644 devel/yaml-cpp/node/node.h %{buildroot}/usr/local/include/yaml-cpp/node/
install -p -D -m 0644 devel/yaml-cpp/node/parse.h %{buildroot}/usr/local/include/yaml-cpp/node/
install -p -D -m 0644 devel/yaml-cpp/node/ptr.h %{buildroot}/usr/local/include/yaml-cpp/node/
install -p -D -m 0644 devel/yaml-cpp/node/type.h %{buildroot}/usr/local/include/yaml-cpp/node/

install -p -D -m 0644 devel/yaml-cpp/node/detail/bool_type.h %{buildroot}/usr/local/include/yaml-cpp/node/detail/
install -p -D -m 0644 devel/yaml-cpp/node/detail/impl.h %{buildroot}/usr/local/include/yaml-cpp/node/detail/
install -p -D -m 0644 devel/yaml-cpp/node/detail/iterator.h %{buildroot}/usr/local/include/yaml-cpp/node/detail/
install -p -D -m 0644 devel/yaml-cpp/node/detail/iterator_fwd.h %{buildroot}/usr/local/include/yaml-cpp/node/detail/
install -p -D -m 0644 devel/yaml-cpp/node/detail/memory.h %{buildroot}/usr/local/include/yaml-cpp/node/detail/
install -p -D -m 0644 devel/yaml-cpp/node/detail/node.h %{buildroot}/usr/local/include/yaml-cpp/node/detail/
install -p -D -m 0644 devel/yaml-cpp/node/detail/node_data.h %{buildroot}/usr/local/include/yaml-cpp/node/detail/
install -p -D -m 0644 devel/yaml-cpp/node/detail/node_iterator.h %{buildroot}/usr/local/include/yaml-cpp/node/detail/
install -p -D -m 0644 devel/yaml-cpp/node/detail/node_ref.h %{buildroot}/usr/local/include/yaml-cpp/node/detail/

%clean
[ "%{buildroot}" != "/" ] && %{__rm} -rf %{buildroot}


%files
%defattr(-,root,root)
/usr/local/lib/libyaml-cpp.so.0.6.3
/usr/local/lib/libyaml-cpp.so.0.6
/usr/local/lib/libyaml-cpp.so
/usr/local/include/yaml-cpp/

%changelog
* Sun Mar  21 2020 Zao Gua <hankai17@126.com>
- yaml-cpp-6.3 first rpm version
