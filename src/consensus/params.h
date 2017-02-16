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
 * Parameters that influence chain consensus.
 */
struct Params {
    uint256 hashGenesisBlock;
    /** Used to check majorities for block version upgrade */
    int nEnforceBlockUpgradeMajority;
    int nRejectBlockOutdatedMajority;
    int nToCheckBlockUpgradeMajority;
    bool fAllowMinDifficultyBlocks;
    bool fDefaultConsistencyChecks;
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
    int64_t nCoinbaseMaturity
    int64_t nStakeMinAge;
    int64_t nStakeMaxAge;
    int64_t nModifierInterval;
};

/** 
 * Modifiable parameters interface is used by test cases to adapt the parameters in order
 * to test specific features more easily. Test cases should always restore the previous
 * values after finalization.
 */

class CModifiableParams {
public:
    //! Published setters to allow changing values in unit test cases
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority)=0;
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority)=0;
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority)=0;
    virtual void setDefaultConsistencyChecks(bool aDefaultConsistencyChecks)=0;
    virtual void setAllowMinDifficultyBlocks(bool aAllowMinDifficultyBlocks)=0;
};

} // namespace Consensus

#endif // SILK_CONSENSUS_PARAMS_H
