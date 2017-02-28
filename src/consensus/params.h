// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SILK_CONSENSUS_PARAMS_H
#define SILK_CONSENSUS_PARAMS_H

#include "uint256.h"

#include <map>
#include <string>

namespace Consensus {

/**
 * Struct for each individual consensus rule change using BIP9.
 */
struct BIP9Deployment {
    /** Bit position to select the particular bit in nVersion. */
    int bit;
    /** Start MedianTime for version bits miner confirmation. Can be a date in the past */
    int64_t nStartTime;
    /** Timeout/expiry MedianTime for the deployment attempt. */
    int64_t nTimeout;
};

/**
 * Parameters that influence chain consensus.
 */
struct Params {
    uint256 hashGenesisBlock;
    /** Used to check majorities for block version upgrade */
    int nEnforceBlockUpgradeMajority;
    int nRejectBlockOutdatedMajority;
    int nToCheckBlockUpgradeMajority;
    bool fAllowMinDifficultyBlocks;
    /** Proof of Work parameters */
    uint256 bnProofOfWorkLimit;
    int64_t nTargetSpacingMax;
    int64_t nPoWTargetSpacing;
    int64_t DifficultyAdjustmentInterval() const { return nTargetSpacingMax / nPoWTargetSpacing; }
    int nMinerThreads;
    int nLastPOWBlock;
    /**Proof of Stake parameters */
    uint256 bnProofOfStakeLimit;
    int64_t nPoSTargetSpacing;
    int64_t nStakeMinAge;
    int64_t nStakeMaxAge;
    int64_t nModifierInterval;
};
} // namespace Consensus

#endif // SILK_CONSENSUS_PARAMS_H
