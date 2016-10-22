// Copyright (c) 2015 The Silk Core developers
// Copyright (c) 2016 The Silk Network developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SILK_ZMQ_ZMQCONFIG_H
#define SILK_ZMQ_ZMQCONFIG_H

#if defined(HAVE_CONFIG_H)
#include "config/silk-config.h"
#endif

#include <stdarg.h>
#include <string>

#if ENABLE_ZMQ
#include <zmq.h>
#endif

#include "primitives/block.h"
#include "primitives/transaction.h"

void zmqError(const char *str);

#endif // SILK_ZMQ_ZMQCONFIG_H
