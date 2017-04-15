Sequence 1.1.0.0
=====================

Intro
---------------------
The Sequence(SEQ) blockchain exists in the Duality binary architecture as an asset and in turn for [Dynamic(DYN)](https://github.com/duality-solutions/dynamic) is its real world interface.

Sequence is a digital currency that enables instant payments to anyone, anywhere in the world. Sequence uses peer-to-peer technology to operate with no central authority (decentralisation): managing transactions and issuing currency (SEQ) are carried out collectively by the Sequence network. Sequence is the name of open source software which enables the use of the currency SEQ.

Setup
---------------------
[Sequence](http://github.com/duality-solutions/sequence) is the original Sequence client and it builds the backbone of the network. However, it downloads and stores the entire history of Sequence transactions (which is currently several GBs); depending on the speed of your computer and network connection, the synchronization process can take anywhere from a few hours to a day or more. Thankfully you only have to do this once. If you would like the process to go faster you can [download the blockchain directly](bootstrap.md).

Running
---------------------
The following are some helpful notes on how to run Sequence on your native platform.

### Unix

You need the Qt4 run-time libraries to run Sequence-Qt. On Debian or Ubuntu:

	sudo apt-get install libqtgui4

Unpack the files into a directory and run:

- bin/32/sequence-qt (GUI, 32-bit) or bin/32/sequenced (headless, 32-bit)
- bin/64/sequence-qt (GUI, 64-bit) or bin/64/sequenced (headless, 64-bit)



### Windows

Unpack the files into a directory, and then run sequence-qt.exe.

### OSX

Drag Sequence-Qt to your applications folder, and then run Sequence-Qt.

### Need Help?

* Ask for help on the [BitcoinTalk](https://bitcointalk.org/) forums.

Building
---------------------
The following are developer notes on how to build Sequence on your native platform. They are not complete guides, but include notes on the necessary libraries, compile flags, etc.

- [OSX Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)

Development
---------------------
The Sequence repo's [root README](https://github.com/duality-solutions/sequence/blob/master/README.md) contains relevant information on the development process and automated testing.

- [Coding Guidelines](coding.md)
- [Multiwallet Qt Development](multiwallet-qt.md)
- [Release Notes](release-notes.md)
- [Release Process](release-process.md)
- [Source Code Documentation (External Link)](https://dev.visucore.com/sequence/doxygen/) ***TODO***
- [Translation Process](translation_process.md)
- [Unit Tests](unit-tests.md)

### Resources
* Discuss on the [BitcoinTalk](https://bitcointalk.org/) forums, in the Development & Technical Discussion board.

### Miscellaneous
- [Assets Attribution](assets-attribution.md)
- [Files](files.md)
- [Tor Support](tor.md)
- [Init Scripts (systemd/upstart/openrc)](init.md)

License
---------------------
Distributed under the [MIT/X11 software license](http://www.opensource.org/licenses/mit-license.php).
This product includes software developed by the OpenSSL Project for use in the [OpenSSL Toolkit](https://www.openssl.org/). This product includes
cryptographic software written by Eric Young ([eay@cryptsoft.com](mailto:eay@cryptsoft.com)), and UPnP software written by Thomas Bernard.
