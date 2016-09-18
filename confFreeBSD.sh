#!/bin/sh -v
CPPFLAGS='-I/usr/local/include/db48/ -I/usr/local/include'
export CPPFLAGS
LDFLAGS='-L/usr/local/lib/db48/ -L/usr/local/lib'
export LDFLAGS
./configure
#./configure --enable-debug
#./configure --enable-debug --with-libs 
