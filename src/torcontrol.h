// Copyright (c) 2015 The BitCoin Core developers
// Copyright (c) 2016 The Silk Network developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

/**
 * Functionality for communicating with Tor.
 */
#ifndef SILK_TORCONTROL_H
#define SILK_TORCONTROL_H

#include "scheduler.h"

extern const std::string DEFAULT_TOR_CONTROL;
static const bool DEFAULT_LISTEN_ONION = false;

void StartTorControl(boost::thread_group& threadGroup, CScheduler& scheduler);
void InterruptTorControl();
void StopTorControl();

#endif /* SILK_TORCONTROL_H */
