// Copyright (c) 2009-2016 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Developers
// Copyright (c) 2013-2016 Emercoin Developers
// Copyright (c) 2015-2016 Silk Network Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SILK_MINER_H
#define SILK_MINER_H

#include <stdint.h>
#include <cstddef>

class CBlock;
class CBlockHeader;
class CBlockIndex;
class CReserveKey;
class CScript;
class CWallet;

struct CBlockTemplate;

namespace boost {
    class thread_group;
} // namespace boost

/** Run the miner threads */
void GenerateSilks(bool fGenerate, CWallet* pwallet, int nThreads);
/** Run the stake minter thread */
void MintStake(boost::thread_group& threadGroup, CWallet* pwallet);
/** Generate a new block, without valid proof-of-work */
CBlockTemplate* CreateNewBlock(const CScript& scriptPubKeyIn);
CBlockTemplate* CreateNewBlockWithKey(CReserveKey& reservekey);
CBlockTemplate* CreateNewPoSBlockWithKey(CReserveKey& reservekey, bool& fPoSRet, CWallet* pwallet);
/** Modify the extranonce in a block */
void IncrementExtraNonce(CBlock* pblock, CBlockIndex* pindexPrev, unsigned int& nExtraNonce);
/** Check mined block */
void UpdateTime(CBlockHeader* block, const CBlockIndex* pindexPrev);
/** Check mined proof-of-work block */
bool CheckWork(CBlock* pblock, CWallet& wallet, CReserveKey& reservekey);
/** Do mining precalculation */
void FormatHashBuffers(CBlock* pblock, char* pmidstate, char* pdata, char* phash1);
/** Base sha256 mining transform */
void SHA256Transform(void* pstate, void* pinput, const void* pinit);

extern double dHashesPerSec;
extern int64_t nHPSTimerStart;

#endif // SILK_MINER_H
