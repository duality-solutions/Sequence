# **Silk (SLK) Version 1.0.0.0**
### Waffle Status: [![Stories in Ready](https://badge.waffle.io/SCDeveloper/DarkSilk-Release-Candidate.png?label=ready&title=Ready)](http://waffle.io/SCDeveloper/DarkSilk-Release-Candidate)

Silk Integration/Staging Tree
================================
![SLK logo](https://scontent-lhr3-1.xx.fbcdn.net/hphotos-xtf1/v/t1.0-9/12669495_621650001334502_3517722285731187914_n.png?oh=1065e770b27e92940d7e0268ae53f404&oe=5741CA59)

**Copyright (c) 2015-2016 Silk Network**

#### What is Silk?
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
* Block Size: 2MB (2X Silk Core)


Silk is a digital currency that enables instant payments to anyone, anywhere in the world. Silk uses peer-to-peer technology over ClearNet to operate with no central authority (centralisation): managing transactions and issuing currency (SLK) are carried out collectively by the Silk network. Silk is the name of open source software which enables the use of the currency SLK.



**MainNet Parameters**
P2P Port = 16666
RPC Port = 16667


**TestNet Parameters**
P2P Port = 16668
RPC Port = 16669


Debian/Ubuntu Linux Daemon Build Instructions
================================================

install dependencies:

    $ sudo apt-get update && sudo apt-get upgrade
    $ sudo apt-get install build-essential libboost-all-dev libssl-dev libcurl4-openssl-dev libminiupnpc-dev libdb++-dev libstdc++6 make

build darksilkd from git:

    $ git clone https://github.com/SilkNetwork/Silk-Core.git silk
    $ cd darksilk/src && make -f makefile.unix USE_UPNP=1
   
install and run darksilkd daemon:

    $ sudo strip silkd && sudo cp ~/silk/src/silkd /usr/bin && cd ~/
    $ silkd

here are a few commands, google for more.

    $ ./silkd getinfo
    $ ./silkd getpeerinfo
    $ ./silkd getmininginfo
    $ ./silkd getstakinginfo
    $ ./silkd getnewaddresss
	

Debian/Ubuntu Linux Qt5 Wallet Build Instructions
================================================

update and install dependencies:

    $ sudo apt-get update && sudo apt-get upgrade
    $ sudo apt-get install make libqt5webkit5-dev libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools qtcreator libprotobuf-dev protobuf-compiler build-essential libboost-dev libboost-all-dev libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libssl-dev libdb++-dev libstdc++6 libminiupnpc-dev libevent-dev libcurl4-openssl-dev git libpng-dev qrencode libqrencode-dev

build darksilk-qt from git:

    $ git clone https://github.com/SilkNetwork/Silk-Core.git silk
    $ cd silk && qmake -qt=qt5 "USE_QRCODE=1" "USE_UPNP=1" && make
 
running the darksilk Qt wallet:

    $ sudo ./silk
