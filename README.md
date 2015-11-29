# **Silkcoin (SLK) Version 2.0.0.0**

Silkcoin Integration/Staging Tree
================================
![SLK logo](https://avatars3.githubusercontent.com/u/16062819?v=3&s=460)

**Copyright (c) 2015 The Silkcoin Developers**

#### What is Silkcoin?
----------------
* Algorithm: Scrypt
* Coin Suffix: SLK
* PoW Period: 10,000 Blocks
* PoW Target Spacing: 60 Seconds
* PoW Difficulty Retarget: 10 Blocks
* PoW Reward per Block: 0 SLK
* Full Confirmation: 10 Blocks
* Maturity: 350 Blocks
* PoS Target Spacing: 64 Seconds
* PoS Difficulty Retarget: 10 Blocks
* PoS Reward: 2 SLK Static PoS Reward
* Minimum Confirmations for Stake: 350 Blocks
* PoS Min: 12 Hours
* PoS Max: Unlimited
* Total Coins: 2,000,000,000 SLK
* Block Size: 2MB (2X Bitcoin Core)


Silkcoin is a digital currency that enables instant payments to anyone, anywhere in the world. Silkcoin uses peer-to-peer technology over ClearNet to operate with no central authority (centralisation): managing transactions and issuing currency (SLK) are carried out collectively by the Silkcoin network. Silkcoin is the name of open source software which enables the use of the currency SLK.



**MainNet Parameters**
P2P Port = 16666
RPC Port = 16667


**TestNet Parameters**
P2P Port = 16668
RPC Port = 16669



Build Instructions for Qt5 Linux Wallet (Ubuntu)
================================================
//Install dependencies via Terminal:

$ sudo apt-get install make libqt5webkit5-dev libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools qtcreator libprotobuf-dev protobuf-compiler build-essential libboost-dev libboost-all-dev libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libssl-dev libdb++-dev libstdc++6 libminiupnpc-dev libevent-dev libcurl4-openssl-dev git libpng-dev qrencode libqrencode-dev

//In terminal navigate to the Silkcoin-2.0 folder:

$ cd /home/Silkcoin-2.0

//Then:

$ qmake -qt=qt5 "USE_QRCODE=1" "USE_UPNP=1"

//Then:

$ make

//This will compile and build the Qt Wallet which takes a little while, please be patient.

//When finished you will have a file called Silkcoin - Simply Double Click

//end of guide



Build Instructions for Terminal Based Linux Wallet (Ubuntu)
===========================================================
//Install dependencies via Terminal:

$ sudo apt-get install build-essential libboost-all-dev libssl-dev libcurl4-openssl-dev libminiupnpc-dev libdb++-dev libstdc++6 make 

//In terminal navigate to the Silkcoin-2.0 folder:

$ cd /home/Silkcoin-2.0/src/

//Enter into the terminal:

$ make -f makefile.unix USE_UPNP=1

//This will produce a file named silkcoind which is the command line instance of Silkcoin

//Now type:

$ strip silkcoind

//When finished you will have a file called silkcoind

//To run Silkcoin

$ ./silkcoind & 

//It will complain about having no silkcoin.conf file, we'll edit the one provided and move it into place

$ cd ..
$ nano silkcoin.conf

//Edit the Username and Password fields to anything you choose (but remember them) then save the file

$ mv silkcoin.conf /home/.silkcoin/
$ cd src/
$ ./silkcoind &

//The server will start. Here are a few commands, google for more.

$ ./silkcoind getinfo
$ ./silkcoind getmininginfo
$ ./silkcoind getnewaddresss

//end of guide