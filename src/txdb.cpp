// Copyright (c) 2009-2019 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Developers
// Copyright (c) 2016-2019 Duality Blockchain Solutions Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include "txdb.h"

#include "spentindex.h"
#include "work.h"
#include "uint256.h"

#include <stdint.h>

#include <boost/thread.hpp>

static const char DB_ADDRESSINDEX = 'a';
static const char DB_ADDRESSUNSPENTINDEX = 'u';
static const char DB_TIMESTAMPINDEX = 's';
static const char DB_SPENTINDEX = 'p';
static const char DB_BLOCK_INDEX = 'b';
static const char DB_COINS = 'c';

using namespace std;

void static BatchWriteCoins(CLevelDBBatch &batch, const uint256 &hash, const CCoins &coins) {
    if (coins.IsPruned())
        batch.Erase(make_pair('c', hash));
    else
        batch.Write(make_pair('c', hash), coins);
}

void static BatchWriteHashBestChain(CLevelDBBatch &batch, const uint256 &hash) {
    batch.Write('B', hash);
}

CCoinsViewDB::CCoinsViewDB(size_t nCacheSize, bool fMemory, bool fWipe) : db(GetDataDir() / "chainstate", nCacheSize, fMemory, fWipe) {
}

bool CCoinsViewDB::GetCoins(const uint256 &txid, CCoins &coins) const {
    return db.Read(make_pair('c', txid), coins);
}

bool CCoinsViewDB::HaveCoins(const uint256 &txid) const {
    return db.Exists(make_pair('c', txid));
}

uint256 CCoinsViewDB::GetBestBlock() const {
    uint256 hashBestChain;
    if (!db.Read('B', hashBestChain))
        return uint256(0);
    return hashBestChain;
}

bool CCoinsViewDB::BatchWrite(CCoinsMap &mapCoins, const uint256 &hashBlock) {
    CLevelDBBatch batch;
    size_t count = 0;
    size_t changed = 0;
    for (CCoinsMap::iterator it = mapCoins.begin(); it != mapCoins.end();) {
        if (it->second.flags & CCoinsCacheEntry::DIRTY) {
            BatchWriteCoins(batch, it->first, it->second.coins);
            changed++;
        }
        count++;
        CCoinsMap::iterator itOld = it++;
        mapCoins.erase(itOld);
    }
    if (hashBlock != uint256(0))
        BatchWriteHashBestChain(batch, hashBlock);

    LogPrint("coindb", "Committing %u changed transactions (out of %u) to coin database...\n", (unsigned int)changed, (unsigned int)count);
    return db.WriteBatch(batch);
}

CBlockTreeDB::CBlockTreeDB(size_t nCacheSize, bool fMemory, bool fWipe) : CLevelDBWrapper(GetDataDir() / "blocks" / "index", nCacheSize, fMemory, fWipe) {
}

bool CBlockTreeDB::WriteBlockIndex(const CDiskBlockIndex& blockindex)
{
    return Write(make_pair('b', blockindex.GetBlockHash()), blockindex);
}

bool CBlockTreeDB::WriteBlockFileInfo(int nFile, const CBlockFileInfo &info) {
    return Write(make_pair('f', nFile), info);
}

bool CBlockTreeDB::ReadBlockFileInfo(int nFile, CBlockFileInfo &info) {
    return Read(make_pair('f', nFile), info);
}

bool CBlockTreeDB::WriteLastBlockFile(int nFile) {
    return Write('l', nFile);
}

bool CBlockTreeDB::WriteReindexing(bool fReindexing) {
    if (fReindexing)
        return Write('R', '1');
    else
        return Erase('R');
}

bool CBlockTreeDB::ReadReindexing(bool &fReindexing) {
    fReindexing = Exists('R');
    return true;
}

bool CBlockTreeDB::ReadLastBlockFile(int &nFile) {
    return Read('l', nFile);
}

bool CCoinsViewDB::GetStats(CCoinsStats &stats) const {
    /* It seems that there are no "const iterators" for LevelDB.  Since we
       only need read operations on it, use a const-cast to get around
       that restriction.  */
    std::unique_ptr<CDBIterator> pcursor(const_cast<CLevelDBWrapper*>(&db)->NewIterator());

    pcursor->SeekToFirst();

    CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
    stats.hashBlock = GetBestBlock();
    ss << stats.hashBlock;
    CAmount nTotalAmount = 0;
    while (pcursor->Valid()) {
        boost::this_thread::interruption_point();
        try {
            std::pair<char, uint256> key;
            if (pcursor->GetKey(key) && key.first == DB_COINS) {
                CCoins coins;
                if (pcursor->GetValue(coins)) {
                    ss << key.second;
                    ss << VARINT(coins.nVersion);
                    ss << (coins.fCoinBase ? 'c' : 'n');
                    ss << VARINT(coins.nHeight);
                    stats.nTransactions++;
                    for (unsigned int i=0; i<coins.vout.size(); i++) {
                        const CTxOut &out = coins.vout[i];
                        if (!out.IsNull()) {
                            stats.nTransactionOutputs++;
                            ss << VARINT(i+1);
                            ss << out;
                            nTotalAmount += out.nValue;
                        }
                    }
                    stats.nSerializedSize += 32 + pcursor->GetValueSize();
                    ss << VARINT(0);
                }
            }
            pcursor->Next();
        } catch (const std::exception& e) {
            return error("%s : Deserialize or I/O error - %s", __func__, e.what());
        }
    }
    stats.nHeight = mapBlockIndex.find(GetBestBlock())->second->nHeight;
    stats.hashSerialized = ss.GetHash();
    stats.nTotalAmount = nTotalAmount;
    return true;
}

bool CBlockTreeDB::ReadTxIndex(const uint256 &txid, CDiskTxPos &pos) {
    return Read(make_pair('t', txid), pos);
}

bool CBlockTreeDB::WriteTxIndex(const std::vector<std::pair<uint256, CDiskTxPos> >&vect) {
    CLevelDBBatch batch;
    for (std::vector<std::pair<uint256,CDiskTxPos> >::const_iterator it=vect.begin(); it!=vect.end(); it++)
        batch.Write(make_pair('t', it->first), it->second);
    return WriteBatch(batch);
}

bool CBlockTreeDB::WriteTimestampIndex(const CTimestampIndexKey& timestampIndex)
{
    //CDBBatch batch(*this);
    CLevelDBBatch batch;
    batch.Write(std::make_pair(DB_TIMESTAMPINDEX, timestampIndex), 0);
    return WriteBatch(batch);
}

bool CBlockTreeDB::ReadTimestampIndex(const unsigned int& high, const unsigned int& low, std::vector<uint256>& hashes)
{
    std::unique_ptr<CDBIterator> pcursor(NewIterator());

    pcursor->Seek(std::make_pair(DB_TIMESTAMPINDEX, CTimestampIndexIteratorKey(low)));

    while (pcursor->Valid()) {
        boost::this_thread::interruption_point();
        std::pair<char, CTimestampIndexKey> key;
        if (pcursor->GetKey(key) && key.first == DB_TIMESTAMPINDEX && key.second.timestamp <= high) {
            hashes.push_back(key.second.blockHash);
            pcursor->Next();
        } else {
            break;
        }
    }

    return true;
}

bool CBlockTreeDB::WriteFlag(const std::string &name, bool fValue) {
    return Write(std::make_pair('F', name), fValue ? '1' : '0');
}

bool CBlockTreeDB::ReadFlag(const std::string &name, bool &fValue) {
    char ch;
    if (!Read(std::make_pair('F', name), ch))
        return false;
    fValue = ch == '1';
    return true;
}

bool CBlockTreeDB::UpdateSpentIndex(const std::vector<std::pair<CSpentIndexKey, CSpentIndexValue> >& vect)
{
    //CDBBatch batch(*this);
    CLevelDBBatch batch;
    for (std::vector<std::pair<CSpentIndexKey, CSpentIndexValue> >::const_iterator it = vect.begin(); it != vect.end(); it++) {
        if (it->second.IsNull()) {
            batch.Erase(std::make_pair(DB_SPENTINDEX, it->first));
        } else {
            batch.Write(std::make_pair(DB_SPENTINDEX, it->first), it->second);
        }
    }
    return WriteBatch(batch);
}

bool CBlockTreeDB::ReadSpentIndex(CSpentIndexKey& key, CSpentIndexValue& value)
{
    return Read(std::make_pair(DB_SPENTINDEX, key), value);
}

bool CBlockTreeDB::UpdateAddressUnspentIndex(const std::vector<std::pair<CAddressUnspentKey, CAddressUnspentValue> >& vect)
{
    //CDBBatch batch(*this);
    CLevelDBBatch batch;
    for (std::vector<std::pair<CAddressUnspentKey, CAddressUnspentValue> >::const_iterator it = vect.begin(); it != vect.end(); it++) {
        if (it->second.IsNull()) {
            batch.Erase(std::make_pair(DB_ADDRESSUNSPENTINDEX, it->first));
        } else {
            batch.Write(std::make_pair(DB_ADDRESSUNSPENTINDEX, it->first), it->second);
        }
    }
    return WriteBatch(batch);
}

bool CBlockTreeDB::ReadAddressUnspentIndex(uint160 addressHash, int type, std::vector<std::pair<CAddressUnspentKey, CAddressUnspentValue> >& unspentOutputs)
{
    std::unique_ptr<CDBIterator> pcursor(NewIterator());

    pcursor->Seek(std::make_pair(DB_ADDRESSUNSPENTINDEX, CAddressIndexIteratorKey(type, addressHash)));

    while (pcursor->Valid()) {
        boost::this_thread::interruption_point();
        std::pair<char, CAddressUnspentKey> key;
        if (pcursor->GetKey(key) && key.first == DB_ADDRESSUNSPENTINDEX && key.second.hashBytes == addressHash) {
            CAddressUnspentValue nValue;
            if (pcursor->GetValue(nValue)) {
                unspentOutputs.push_back(std::make_pair(key.second, nValue));
                pcursor->Next();
            } else {
                return error("failed to get address unspent value");
            }
        } else {
            break;
        }
    }

    return true;
}


bool CBlockTreeDB::WriteAddressIndex(const std::vector<std::pair<CAddressIndexKey, CAmount> >& vect)
{
    CLevelDBBatch batch;
    for (std::vector<std::pair<CAddressIndexKey, CAmount> >::const_iterator it = vect.begin(); it != vect.end(); it++)
        batch.Write(std::make_pair(DB_ADDRESSINDEX, it->first), it->second);
    return WriteBatch(batch);
}

bool CBlockTreeDB::EraseAddressIndex(const std::vector<std::pair<CAddressIndexKey, CAmount> >& vect)
{
    CLevelDBBatch batch;
    for (std::vector<std::pair<CAddressIndexKey, CAmount> >::const_iterator it = vect.begin(); it != vect.end(); it++)
        batch.Erase(std::make_pair(DB_ADDRESSINDEX, it->first));
    return WriteBatch(batch);
}

bool CBlockTreeDB::ReadAddressIndex(uint160 addressHash, int type, std::vector<std::pair<CAddressIndexKey, CAmount> >& addressIndex, int start, int end)
{
    std::unique_ptr<CDBIterator> pcursor(NewIterator());
    if (start > 0 && end > 0) {
        pcursor->Seek(std::make_pair(DB_ADDRESSINDEX, CAddressIndexIteratorHeightKey(type, addressHash, start)));
    } else {
        pcursor->Seek(std::make_pair(DB_ADDRESSINDEX, CAddressIndexIteratorKey(type, addressHash)));
    }

    while (pcursor->Valid()) {
        boost::this_thread::interruption_point();
        std::pair<char, CAddressIndexKey> key;
        if (pcursor->GetKey(key) && key.first == DB_ADDRESSINDEX && key.second.hashBytes == addressHash) {
            if (end > 0 && key.second.blockHeight > end) {
                break;
            }
            CAmount nValue;
            if (pcursor->GetValue(nValue)) {
                addressIndex.push_back(std::make_pair(key.second, nValue));
                pcursor->Next();
            } else {
                return error("failed to get address index value");
            }
        } else {
            break;
        }
    }

     return true;
}

bool CBlockTreeDB::LoadBlockIndexGuts()
{
    std::unique_ptr<CDBIterator> pcursor(NewIterator());

    CDataStream ssKeySet(SER_DISK, CLIENT_VERSION);
    ssKeySet << make_pair(DB_BLOCK_INDEX, uint256(0));
    pcursor->Seek(ssKeySet.str());

    // Load mapBlockIndex
    while (pcursor->Valid()) {
        boost::this_thread::interruption_point();
        try {
            std::pair<char, uint256> key;
            if (pcursor->GetKey(key) && key.first == DB_BLOCK_INDEX) {
                CDiskBlockIndex diskindex;
                if (pcursor->GetValue(diskindex)) {

                    // Construct block index object
                    CBlockIndex* pindexNew = InsertBlockIndex(diskindex.GetBlockHash());
                    pindexNew->pprev          = InsertBlockIndex(diskindex.hashPrev);
                    pindexNew->nHeight        = diskindex.nHeight;
                    pindexNew->nFile          = diskindex.nFile;
                    pindexNew->nDataPos       = diskindex.nDataPos;
                    pindexNew->nUndoPos       = diskindex.nUndoPos;
                    pindexNew->nVersion       = diskindex.nVersion;
                    pindexNew->hashMerkleRoot = diskindex.hashMerkleRoot;
                    pindexNew->nTime          = diskindex.nTime;
                    pindexNew->nBits          = diskindex.nBits;
                    pindexNew->nNonce         = diskindex.nNonce;
                    pindexNew->nStatus        = diskindex.nStatus;
                    pindexNew->nTx            = diskindex.nTx;

                    // ppcoin related block index fields
                    pindexNew->nMint          = diskindex.nMint;
                    pindexNew->nMoneySupply   = diskindex.nMoneySupply;
                    pindexNew->nFlags         = diskindex.nFlags;
                    pindexNew->nStakeModifier = diskindex.nStakeModifier;
                    pindexNew->prevoutStake   = diskindex.prevoutStake;
                    pindexNew->nStakeTime     = diskindex.nStakeTime;
                    pindexNew->hashProofOfStake = diskindex.hashProofOfStake;

                    // Sequence: we ignore this check if header was gotten from old client and we still did not download block for it.
                    if (pindexNew->IsProofOfWork() && !CheckProofOfWork(pindexNew->GetBlockHash(), pindexNew->nBits, Params().GetConsensus()))
                        return error("LoadBlockIndexGuts() : CheckProofOfWork failed: %s", pindexNew->ToString());

                    pcursor->Next();
                } else {
                    return error("LoadBlockIndex() : failed to read value");
                }
            } else {
                break; // if shutdown requested or finished loading block index
            }
        } catch (const std::exception& e) {
            return error("%s : Deserialize or I/O error - %s", __func__, e.what());
        }
    }

    return true;
}

bool CBlockTreeDB::ReadSyncCheckpoint(uint256& hashCheckpoint)
{
    return Read(string("hashSyncCheckpoint"), hashCheckpoint);
}

bool CBlockTreeDB::WriteSyncCheckpoint(uint256 hashCheckpoint)
{
    return Write(string("hashSyncCheckpoint"), hashCheckpoint);
}

bool CBlockTreeDB::ReadCheckpointPubKey(string& strPubKey)
{
    return Read(string("strCheckpointPubKey"), strPubKey);
}

bool CBlockTreeDB::WriteCheckpointPubKey(const string& strPubKey)
{
    return Write(string("strCheckpointPubKey"), strPubKey);
}
