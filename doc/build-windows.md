WINDOWS BUILD NOTES
====================

Some notes on how to build Silk Core for Windows.

Most developers use cross-compilation from Ubuntu to build executables for
Windows. This is also used to build the release binaries.

Building on Windows itself is possible (for example using msys / mingw-w64),
but no one documented the steps to do this. If you are doing this, please contribute them.

Cross-compilation
-------------------

These steps can be performed on, for example, an Ubuntu VM. The depends system
will also work on other Linux distributions, however the commands for
installing the toolchain will be different.

First install depends:

    sudo apt-get install -y build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils
    sudo apt-get install -y libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev libboost-all-dev
    sudo add-apt-repository ppa:silknetwork/silknetwork && sudo apt-get update && sudo apt-get install -y libdb4.8-dev libdb4.8++-dev
    sudo apt-get install -y git libminiupnpc-dev libzmq3-dev libqt4-dev libprotobuf-dev protobuf-compiler libqrencode-dev libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libcrypto++-dev

Second install the toolchains:

    sudo apt-get install g++-mingw-w64-i686 mingw-w64-i686-dev g++-mingw-w64-x86-64 mingw-w64-x86-64-dev

To build executables for Windows 32-bit:

    cd depends
    make HOST=i686-w64-mingw32 -j2
    cd ..
    ./autogen.sh && ./configure --prefix=`pwd`/depends/i686-w64-mingw32
    make

To build executables for Windows 64-bit:

    cd depends
    make HOST=x86_64-w64-mingw32 -j2
    cd ..
    ./autogen.sh && ./configure --prefix=`pwd`/depends/x86_64-w64-mingw32
    make

For further documentation on the depends system see [README.md](../depends/README.md) in the depends directory.

