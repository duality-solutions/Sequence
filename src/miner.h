// Copyright (c) 2009-2017 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Developers
// Copyright (c) 2013-2017 Emercoin Developers
// Copyright (c) 2016-2017 Duality Blockchain Solutions Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SEQUENCE_MINER_H
#define SEQUENCE_MINER_H

#include <primitives/block.h>
#include <consensus/params.h>

#include <cstdint>
#include <cstddef>

class CBlockHeader;
class CBlockIndex;
class CChainParams;
class CReserveKey;
class CScript;
class CWallet;

namespace boost {
    class thread_group;
} // namespace boost

static const bool DEFAULT_GENERATE = false;
static const int DEFAULT_GENERATE_THREADS = 1;

struct CBlockTemplate
{
    CBlock block;
    std::vector<CAmount> vTxFees;
    std::vector<int64_t> vTxSigOps;
};

/** Run the miner threads */
void GenerateSequences(bool fGenerate, CWallet* pwallet, int nThreads, const CChainParams& chainparams);
/** Run the stake minter thread */
void MintStake(boost::thread_group& threadGroup, CWallet* pwallet);
/** Generate a new block, without valid proof-of-work */
CBlockTemplate* CreateNewBlock(const CScript& scriptPubKeyIn);
CBlockTemplate* CreateNewBlockWithKey(CReserveKey& reservekey);
CBlockTemplate* CreateNewPoSBlockWithKey(CReserveKey& reservekey, bool& fPoSRet, CWallet* pwallet);
/** Modify the extranonce in a block */
void IncrementExtraNonce(CBlock* pblock, CBlockIndex* pindexPrev, unsigned int& nExtraNonce);
/** Check mined block */
int64_t UpdateTime(CBlockHeader* pblock, const Consensus::Params& consensusParams, const CBlockIndex* pindexPrev);
/** Check mined proof-of-work block */
bool CheckWork(const CChainParams& chainparams, CBlock* pblock, CWallet& wallet, CReserveKey& reservekey);
/** Do mining precalculation */
void FormatHashBuffers(CBlock* pblock, char* pmidstate, char* pdata, char* phash1);
/** Base sha256 mining transform */
void SHA256Transform(void* pstate, void* pinput, const void* pinit);

extern double dHashesPerSec;
extern int64_t nHPSTimerStart;

#endif // SEQUENCE_MINER_H
