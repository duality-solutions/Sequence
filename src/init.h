// Copyright (c) 2009-2017 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Developers
// Copyright (c) 2015-2017 Silk Network Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SILK_INIT_H
#define SILK_INIT_H

#include <string>

class CWallet;
class CScheduler;

namespace boost
{
class thread_group;
} // namespace boost

extern CWallet* pwalletMain;

void StartShutdown();
bool ShutdownRequested();
void Shutdown();
void PrepareShutdown();
bool AppInit2(boost::thread_group& threadGroup, CScheduler& scheduler);

/** The help message mode determines what help message to show */
enum HelpMessageMode {
    HMM_SILKD,
    HMM_SILK_QT
};

/** Help for options shared between UI and daemon (for -help) */
std::string HelpMessage(HelpMessageMode mode);
/** Returns licensing information (for -version) */
std::string LicenseInfo();

#endif // SILK_INIT_H
