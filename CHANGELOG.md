**Sequence CHANGELOG**
-------------------------

**Sequence v1.2.0.0**

* Fix Send Confirmation Window
* Bump Proto/Version
* Copyrights
* Make fee more for splitblock
* Remove CLIENT_DATE and cleanup
* Remove MultiSigUI

**Sequence v1.1.0.0**

* Fix sequence.conf read on daemon first launch
* Overall UI Improvements
* Staking Icon now shows Status/MultiSend Status/Weight/Network Weight/Money Supply/Next Reward within
* Stake Network Weight Fix
* Improtve DecodeBase58/EncodeBase58
* Repair Signal/Slot in stakereport.cpp
* Add Checkpoint/Stake Modifier
* Add Scammer Warning
* Add OSX keystroke to clear RPCConsole
* Fix tabs
* Remove unused functions
* Stake Report
* net: Avoid duplicate getheaders requests.
* Update DB_CORRUPT message
* HD Wallet
* Improve modaloverlay and messages
* Change pause progresslabel to clear once synced
* Repair SendCoinsDialog::setClientModel Signal Slot
* Upgrade Crypter and implement LockedPool
* Repair Traffic Graph
* Fully implement indirectmap
* Alphabetically sort all makefiles and qrc
* Update LevelDB to 1.19 and implement makefile.leveldb.include
* Fix Signal/Slot errors
* Remove embedded colour profiles from all images
* c++11:Backport from bitcoin-core: don't throw from the reverselock destructor
* Fix LOCK in LOCK
* Update net.cpp/h and implement CopyNodeVector/ReleaseNodeVector
* Do not shadow global variable
* Do not shadow upper local variable 'send', prevent -Wshadow compiler 
* Fix Locks and Do not add random inbound peers to addrman
* Fix calculation of number of bound sockets to use
* Fix races
* Add/Repair LOCK's
* Fix parameter naming inconsistencies
* Fix torcontrol.cpp unused private field warning
* Fix unlocked access to vNodes.size()
* Don't return the address of a P2SH of a P2SH
* trivial: fix bloom filter init to isEmpty = true
* Avoid ugly exception in log on unknown inv type
* Make strWalletFile const
* Make RelayWalletTransaction attempt to AcceptToMemoryPool.
* Locking for Misbehave() and other cs_main locking fixes
* Update Stake Icon to show useful accurate information


** Sequence v1.0.0.0**

* Fix and re-enable ResetSyncCheckpoint()
* Fix checkpoint locks
* Fix 'Stake only mode' checkbox styling
* Add comments for pubkeys/privkeys in chainparams.cpp
* Fix all includes and paths
* Align Ping Time text to the left to make it easier to read
* Changed Address Label in SendCoinsDialog to display in white
* Style UI checkboxes
* Add non-mainnet units and amend THIN spacing/HTML code
* Convert all emit instances in src/qt/ to Q_EMIT
* Improvements for Font and Icon Display in Qt
* Fixed libraries for Gitian
* Fixed Gitian script files
* Upgraded dependency packages
* Windows installer creator script
* Improve GUI, shortcuts and toolbar menu
* Wallet Autobackup Added
* Refactored chainparams
* moved g.signals into validation interface
* Overhauled net.cpp/h netbase.cpp/h
* SanitizeString implemented
* params.h introduced and consensus parameters from chainparams added
* Bantablemodel and peers table contextual menu to enable UI elements for node banning
* Open sequence.conf via UI added
* Mempool usage display fix
* Fix QMetaObject::invokeMethod for ClientModel::numBlocksChanged(int)
* Fix CPU utilisation caused by network issues
* Implement HD Wallet
* Change all public, private and protected slots in src/qt/ to Q_SLOTS
* Fixed missing icons in Qt Makefile
* Timedata: Added lock-free time-diff extraction
* Fix Possible Deadlocks
* Fix AND/OR bug
* Added Release() after nodes usage, and iterate vNodesCopy
* Add block hash to stake hash
* ddns: Fixed minor bug: send extra 10 bytes, if incorrect IPV4 address
* Check block before signing
* Bump Version
* Alert Key Added
* OpenSSL 1.1 API Fix
* Fixed BIP68 Test
* Updated byteswap.h
* Make connection icon clickable
* Fix amount discrepancies (%08d -> %06d)
* Increase block version to 2 for CheckLockTimeVerify OP. 
* ENUM for dDNS.
* Increased DAP limit 10x, and activate DAP only when public GW.
* net.cpp and netbase.cpp fixes. 
* minfee sending fix for Sequence Qt. 
* Currency display fix. 
* Gitian descriptor fixes.
* Addition of needed function in dDNS for Windows builds. 
* Suppression/Fixes of warnings in seqdns.cpp
* Update protobuf version.
* [RPC]add legacy json mode (enabled by default, enable unicode by adding -legacyrpc=0 to sequence.conf).
* [RPC]add hex and base64 output to the rest on name commands.
* [RPC]add hex and base64 input/output to name_show, name_new, name_update.
* Fix Crash in GetStake().
* Add checkpoints and stake modifier checkpoints.
* Fix MacOSX build error
* Add block and stake modifier checkpoints
* Set minimum required Boost to 1.47.0
* Replaces deprecated auto_ptr with unique_ptr
* Removes scoped enums declaration (now redundant)
* Adds c++11 compatability and adds support for bigendian
* Implements support for bigendian
* Remove startup flags from auto-generated conf
* Implement C++11 Compiling Capabilities
* Fix Qt project file so we can run the debugger with the UI
* mapNextTx: Use pointer as key, simplify value
* Add RPCConsole to SequenceGUI::closeEvent
* Update OSX Build Instructions
* Move CAddrDB out of net.h/.cpp
* Add conf file if file is not found
* Fix memory use display issue
* Add checkpoint for end of PoW phase
* Fix checkpoint warnings
* Update main PoS min maturity time
* New Kernel Stake Modifier
* Add Qt project for degugging in Qt Creator
* Update to base58prefixes to vectors
* Addition of Checkpoint for Block 4000
* Add stake modifier checkpoint for block 2000
* Add v1.0.0.0 release notes
* Add Windows build document
* Change menu font color for more contrast
* Fix timer kill error and shutdown delay
* Update Travis with all dependencies
* Update for Qt5
* Make Travis-CI test with Qt5, not Qt4
* Update to latest leveldb code
* Fixes sync height in peer table while chain downloading.
* Update Sequence readme.
* Updates PRT.
* Adds issue template to repo.

** Initial Fork from Emercoin 
