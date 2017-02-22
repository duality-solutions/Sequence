Silk-Core v1.2.0.0 Changelog

Wallet Autobackup Added
Refactored chainparams
moved g.signals into validation interface
Overhauled net.cpp/h netbase.cpp/h
SanitizeString implemented
params.h introduced and consensus parameters from chainparams added
Bantablemodel and peers table contextual menu to enable UI elements for node banning
Open silk.conf via UI added
Mempool usage display fix
Fix QMetaObject::invokeMethod for ClientModel::numBlocksChanged(int)
Fix CPU utilisation caused by network issues
Implement HD Wallet
Change all public, private and protected slots in src/qt/ to Q_SLOTS



Silk-Core v1.1.1.0 Changelog

Timedata: Added lock-free time-diff extraction
Fix Possible Deadlocks
Fix AND/OR bug
Added Release() after nodes usage, and iterate vNodesCopy
Add block hash to stake hash
ddns: Fixed minor bug: send extra 10 bytes, if incorrect IPV4 address
Check block before signing
Bump Version
Alert Key Added
OpenSSL 1.1 API Fix
Fixed BIP68 Test
Updated byteswap.h
Make connection icon clickable
Fix amount discrepancies (%08d -> %06d)



Silk-Core 1.1.0.0 Changelog

Increase block version to 2 for CheckLockTimeVerify OP. 
ENUM for dDNS.
Increased DAP limit 10x, and activate DAP only when public GW.
net.cpp and netbase.cpp fixes. 
mifee sending fix for Silk-Core Qt. 
Currency display fix. 
Gitian descriptor fixes.
Addition of needed function in dDNS for Windows builds. 
Suppression/Fixes of warnings in slkdns.cpp
Update protobuf version.
[RPC]add legacy json mode (enabled by default, enable unicode by adding -legacyrpc=0 to silk.conf).
[RPC]add hex and base64 output to the rest on name commands.
[RPC]add hex and base64 input/output to name_show, name_new, name_update.
Fix Crash in GetStake().
Add checkpoints and stake modifier checkpoints.



Silk-Core v1.0.1.1 Changelog

Fix MacOSX build error
Add block and stake modifier checkpoints
Set minimum required Boost to 1.47.0
Replaces deprecated auto_ptr with unique_ptr
Removes scoped enums declaration (now redundant)
Adds c++11 compatability and adds support for bigendian
Implements support for bigendian
Remove startup flags from auto-generated conf
Implement C++11 Compiling Capabilities
Fix Qt project file so we can run the debugger with the UI
mapNextTx: Use pointer as key, simplify value
Add RPCConsole to SilkGUI::closeEvent



Silk-Core v1.0.1.0 Changelog

Update OSX Build Instructions
Move CAddrDB out of net.h/.cpp
Add conf file if file is not found
Fix memory use display issue
Add checkpoint for end of PoW phase
Fix checkpoint warnings
Update main PoS min maturity time
New Kernel Stake Modifier
Add Qt project for degugging in Qt Creator
Update to base58prefixes to vectors
Adds Silk Network PPA
Addition of Checkpoint for Block 4000
Add stake modifier checkpoint for block 2000



Silk-Core v1.0.0.0 Changelog

Add v1.0.0.0 release notes
Add Windows build document
Change menu font color for more contrast
Fix timer kill error and shutdown delay
Update Travis with all dependencies
Update for Qt5
Make Travis-CI test with Qt5, not Qt4
Update to latest leveldb code
Fixes sync height in peer table while chain downloading.
Update silkcoin readme.
Updates PRT.
Adds issue template to repo.



