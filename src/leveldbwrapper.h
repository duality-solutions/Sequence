// Copyright (c) 2009-2019 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Developers
// Copyright (c) 2016-2019 Duality Blockchain Solutions Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SEQUENCE_LEVELDBWRAPPER_H
#define SEQUENCE_LEVELDBWRAPPER_H

#include "clientversion.h"
#include "serialize.h"
#include "streams.h"
#include "util.h"
#include "version.h"

#include <boost/filesystem/path.hpp>

#include <leveldb/db.h>
#include <leveldb/write_batch.h>

static const size_t DBWRAPPER_PREALLOC_KEY_SIZE = 64;

class dbwrapper_error : public std::runtime_error
{
public:
    dbwrapper_error(const std::string& msg) : std::runtime_error(msg) {}
};

class leveldb_error : public std::runtime_error
{
public:
    leveldb_error(const std::string& msg) : std::runtime_error(msg) {}
};

void HandleError(const leveldb::Status& status) throw(leveldb_error);

class CLevelDBWrapper;
//class CDBIterator;

/** These should be considered an implementation detail of the specific database.
 */
namespace dbwrapper_private
{
/** Handle database error by throwing dbwrapper_error exception.
 */
//void HandleError(const leveldb::Status& status);

/** Work around circular dependency, as well as for testing in dbwrapper_tests.
 * Database obfuscation should be considered an implementation detail of the
 * specific database.
 */
const std::vector<unsigned char>& GetObfuscateKey(const CLevelDBWrapper& w);

}; // namespace dbwrapper_private

/** Batch of changes queued to be written to a CLevelDBWrapper */
class CLevelDBBatch
{
    friend class CLevelDBWrapper;

private:
    leveldb::WriteBatch batch;

public:
    template <typename K, typename V>
    void Write(const K& key, const V& value)
    {
        CDataStream ssKey(SER_DISK, CLIENT_VERSION);
        ssKey.reserve(ssKey.GetSerializeSize(key));
        ssKey << key;
        leveldb::Slice slKey(&ssKey[0], ssKey.size());

        CDataStream ssValue(SER_DISK, CLIENT_VERSION);
        ssValue.reserve(ssValue.GetSerializeSize(value));
        ssValue << value;
        leveldb::Slice slValue(&ssValue[0], ssValue.size());

        batch.Put(slKey, slValue);
    }

    template <typename K>
    void Erase(const K& key)
    {
        CDataStream ssKey(SER_DISK, CLIENT_VERSION);
        ssKey.reserve(ssKey.GetSerializeSize(key));
        ssKey << key;
        leveldb::Slice slKey(&ssKey[0], ssKey.size());

        batch.Delete(slKey);
    }
};

class CDBIterator
{
private:
    const CLevelDBWrapper& parent;
    leveldb::Iterator* piter;

public:
    /**
     * @param[in] parent           Parent CDBWrapper instance.
     */
    CDBIterator(const CLevelDBWrapper& parent, leveldb::Iterator* piterIn) : parent(parent), piter(piterIn){};
    ~CDBIterator();

    bool Valid();

    void SeekToFirst();
    void SeekToLast();

    template <typename K>
    void Seek(const K& key)
    {
        CDataStream ssKey(SER_DISK, CLIENT_VERSION);
        ssKey.reserve(DBWRAPPER_PREALLOC_KEY_SIZE);
        ssKey << key;
        leveldb::Slice slKey(ssKey.data(), ssKey.size());
        piter->Seek(slKey);
    }

    void Next();

    template <typename K>
    bool GetKey(K& key)
    {
        leveldb::Slice slKey = piter->key();
        try {
            CDataStream ssKey(slKey.data(), slKey.data() + slKey.size(), SER_DISK, CLIENT_VERSION);
            ssKey >> key;
        } catch (const std::exception&) {
            return false;
        }
        return true;
    }

    template <typename V>
    bool GetValue(V& value)
    {
        leveldb::Slice slValue = piter->value();
        try {
            CDataStream ssValue(slValue.data(), slValue.data() + slValue.size(), SER_DISK, CLIENT_VERSION);
            ssValue.Xor(dbwrapper_private::GetObfuscateKey(parent));
            ssValue >> value;
        } catch (const std::exception&) {
            return false;
        }
        return true;
    }

    unsigned int GetValueSize()
    {
        return piter->value().size();
    }
};



class CLevelDBWrapper
{
private:
    //! custom environment this database is using (may be NULL in case of default environment)
    leveldb::Env* penv;

    //! database options used
    leveldb::Options options;

    //! options used when reading from the database
    leveldb::ReadOptions readoptions;

    //! options used when iterating over values of the database
    leveldb::ReadOptions iteroptions;

    //! options used when writing to the database
    leveldb::WriteOptions writeoptions;

    //! options used when sync writing to the database
    leveldb::WriteOptions syncoptions;

    //! the database itself
    leveldb::DB* pdb;

public:
    CLevelDBWrapper(const boost::filesystem::path& path, size_t nCacheSize, bool fMemory = false, bool fWipe = false);
    ~CLevelDBWrapper();

    //! a key used for optional XOR-obfuscation of the database
    std::vector<unsigned char> obfuscate_key;



    template <typename K, typename V>
    bool Read(const K& key, V& value) const throw(leveldb_error)
    {
        CDataStream ssKey(SER_DISK, CLIENT_VERSION);
        ssKey.reserve(ssKey.GetSerializeSize(key));
        ssKey << key;
        leveldb::Slice slKey(&ssKey[0], ssKey.size());

        std::string strValue;
        leveldb::Status status = pdb->Get(readoptions, slKey, &strValue);
        if (!status.ok()) {
            if (status.IsNotFound())
                return false;
            LogPrintf("LevelDB read failure: %s\n", status.ToString());
            HandleError(status);
        }
        try {
            CDataStream ssValue(strValue.data(), strValue.data() + strValue.size(), SER_DISK, CLIENT_VERSION);
            ssValue >> value;
        } catch (const std::exception&) {
            return false;
        }
        return true;
    }

    template <typename K, typename V>
    bool Write(const K& key, const V& value, bool fSync = false) throw(leveldb_error)
    {
        CLevelDBBatch batch;
        batch.Write(key, value);
        return WriteBatch(batch, fSync);
    }

    template <typename K>
    bool Exists(const K& key) const throw(leveldb_error)
    {
        CDataStream ssKey(SER_DISK, CLIENT_VERSION);
        ssKey.reserve(ssKey.GetSerializeSize(key));
        ssKey << key;
        leveldb::Slice slKey(&ssKey[0], ssKey.size());

        std::string strValue;
        leveldb::Status status = pdb->Get(readoptions, slKey, &strValue);
        if (!status.ok()) {
            if (status.IsNotFound())
                return false;
            LogPrintf("LevelDB read failure: %s\n", status.ToString());
            HandleError(status);
        }
        return true;
    }

    template <typename K>
    bool Erase(const K& key, bool fSync = false) throw(leveldb_error)
    {
        CLevelDBBatch batch;
        batch.Erase(key);
        return WriteBatch(batch, fSync);
    }

    bool WriteBatch(CLevelDBBatch& batch, bool fSync = false) throw(leveldb_error);

    // not available for LevelDB; provide for compatibility with BDB
    bool Flush()
    {
        return true;
    }

    bool Sync() throw(leveldb_error)
    {
        CLevelDBBatch batch;
        return WriteBatch(batch, true);
    }

    // not exactly clean encapsulation, but it's easiest for now
    leveldb::Iterator* NewIterator()
    {
        return pdb->NewIterator(iteroptions);
    }

    CDBIterator* NewIterator2()
    {
        return new CDBIterator(*this, pdb->NewIterator2(iteroptions));
    }    
};





namespace dbwrapper_private
{
// void HandleError(const leveldb::Status& status)
// {
//     if (status.ok())
//         return;
//     LogPrintf("%s\n", status.ToString());
//     if (status.IsCorruption())
//         throw dbwrapper_error("Database corrupted");
//     if (status.IsIOError())
//         throw dbwrapper_error("Database I/O error");
//     if (status.IsNotFound())
//         throw dbwrapper_error("Database entry missing");
//     throw dbwrapper_error("Unknown database error");
// }

const std::vector<unsigned char>& GetObfuscateKey(const CLevelDBWrapper& w)
{
    return w.obfuscate_key;
}

}; // namespace dbwrapper_private

#endif // SEQUENCE_LEVELDBWRAPPER_H
