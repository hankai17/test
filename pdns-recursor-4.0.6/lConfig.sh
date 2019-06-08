#!/bin/bash

#./configure LDFLAGS="$LDFLAGS -lcurl" --prefix=/usr --sysconfdir="/etc/icdns/"  --with-luajit --with-lua=no --enable-hardening=yes CFLAGS="-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -m64 -mtune=generic" CXXFLAGS="-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -m64 -mtune=generic -Wno-deprecated" PKG_CONFIG_PATH=":/usr/lib64/pkgconfig:/usr/share/pkgconfig" --with-boost=/usr/local/boost_1_63_0/ LUA_LIBS="/usr/local/lib/libluajit-5.1.so" LUA_CFLAGS="-L/usr/local/lib -lluajit"
#./configure LDFLAGS="$LDFLAGS -lcurl" --prefix=/usr --sysconfdir="/etc/icdns/" --enable-hardening=yes  --with-protobuf=no CFLAGS="-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -m64 -mtune=generic" CXXFLAGS="-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -m64 -mtune=generic -Wno-deprecated" PKG_CONFIG_PATH=":/usr/lib64/pkgconfig:/usr/share/pkgconfig" --with-boost=/usr/local/boost_1_63_0/
#./configure LDFLAGS="$LDFLAGS -lcurl" --prefix=/usr --sysconfdir="/etc/icdns/" --enable-hardening=yes  --with-protobuf=no CFLAGS="-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -m64 -mtune=generic" CXXFLAGS="-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -m64 -mtune=generic -Wno-deprecated" PKG_CONFIG_PATH=":/usr/lib64/pkgconfig:/usr/share/pkgconfig" --with-boost=/usr/local
./configure LDFLAGS="$LDFLAGS -lcurl" --prefix=/usr --sysconfdir="/etc/icdns/" --enable-hardening=yes --with-luajit --with-lua=yes --with-protobuf=no CFLAGS="-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -m64 -mtune=generic" CXXFLAGS="-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -m64 -mtune=generic -Wno-deprecated" PKG_CONFIG_PATH=":/usr/lib64/pkgconfig:/usr/share/pkgconfig" --with-boost=/usr/local LUA_LIBS="/usr/local/src/LuaJIT-2.0.2/src/libluajit.so" LUA_CFLAGS="-L/usr/local/lib -lluajit -I/usr/local/src/LuaJIT-2.0.2/src/"