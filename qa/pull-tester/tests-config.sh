#!/bin/bash
# Copyright (c) 2013-2014 The Bitcoin Developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

BUILDDIR="/home/empinel/Silk-Core"
EXEEXT=""

# These will turn into comments if they were disabled when configuring.
ENABLE_WALLET=1
ENABLE_UTILS=1
ENABLE_SILKD=1

REAL_SILKD="$BUILDDIR/src/silkd${EXEEXT}"
REAL_SILKCLI="$BUILDDIR/src/silk-cli${EXEEXT}"

