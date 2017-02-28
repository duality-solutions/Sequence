// Copyright (c) 2009-2017 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Developers
// Copyright (c) 2015-2017 Silk Network Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "chainparamsseeds.h"
#include "consensus/merkle.h"
#include "random.h"
#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "uint256.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

void MineGenesis(CBlock genesis, uint256 bnProofOfWorkLimit){
    // This will figure out a valid hash and Nonce if you're creating a different genesis block:
    uint256 hashTarget = bnProofOfWorkLimit;
    printf("Target: %s\n", hashTarget.GetHex().c_str());
    uint256 newhash = genesis.GetHash();
    uint256 besthash;
    memset(&besthash,0xFF,32);
    while (newhash > hashTarget) {
        ++genesis.nNonce;
        if (genesis.nNonce == 0){
            printf("NONCE WRAPPED, incrementing time");
            ++genesis.nTime;
        }
    newhash = genesis.GetHash();
    if(newhash < besthash){
        besthash=newhash;
        printf("New best: %s\n", newhash.GetHex().c_str());
    }
    }
    printf("Genesis Hash: %s\n", genesis.GetHash().ToString().c_str());
    printf("Genesis Hash Merkle: %s\n", genesis.hashMerkleRoot.ToString().c_str());
    printf("Genesis nTime: %u\n", genesis.nTime);
    printf("Genesis nBits: %08x\n", genesis.nBits);
    printf("Genesis Nonce: %u\n\n\n", genesis.nNonce);
}

/**
 * Main network
 */
static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    std::vector<CTxIn> vin;
    vin.resize(1);
    vin[0].scriptSig = CScript() << 1473949500 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    std::vector<CTxOut> vout;
    vout.resize(1);
    vout[0].scriptPubKey = genesisOutputScript;
    vout[0].nValue = 0;
    CMutableTransaction txNew(1, 1473949300, vin, vout, 0);

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock = 0;
    genesis.hashMerkleRoot = genesis.BuildMerkleTree();

    return genesis;
}

static CBlock CreateTestNetGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    std::vector<CTxIn> vin;
    vin.resize(1);
    vin[0].scriptSig = CScript() << 1473949300 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    std::vector<CTxOut> vout;
    vout.resize(1);
    vout[0].scriptPubKey = genesisOutputScript;
    vout[0].nValue = 0;
    CMutableTransaction txNew(1, 1478106900, vin, vout, 0);

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock = 0;
    genesis.hashMerkleRoot = genesis.BuildMerkleTree();

    return genesis;
}

static CBlock CreateTestNetGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "AP November 2nd 2016: Telescope reveals amazing pillars in star-breeding ground";
    const CScript genesisOutputScript = CScript() << ParseHex("") << OP_CHECKSIG;
    return CreateTestNetGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "NY Times Sept 15th 2016: Pardon Edward Snowden";
    const CScript genesisOutputScript = CScript() << ParseHex("") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.bnProofOfWorkLimit = ~uint256(0) >> 26;
        consensus.bnProofOfStakeLimit = ~uint256(0) >> 26;
        consensus.nEnforceBlockUpgradeMajority = 750;
        consensus.nRejectBlockOutdatedMajority = 950;
        consensus.nToCheckBlockUpgradeMajority = 1000;
        consensus.nMinerThreads = 0;
        consensus.nTargetSpacingMax = 1 * 64;     // 64 second max spacing target
        consensus.nPoWTargetSpacing = 1 * 60;     // 60 seconds PoW Target
        consensus.fAllowMinDifficultyBlocks = false;
        consensus.nPoSTargetSpacing = 1 * 64;     // 64 seconds PoS Target
        consensus.nStakeMinAge = 1 * 60 * 60;     // 1 hour minimum stake age
        consensus.nStakeMaxAge = std::numeric_limits<int64_t>::max(); // Unlimited stake age
        consensus.nModifierInterval = 15 * 60;    // 15 minutes to elapse before new modifier is computed
        consensus.nLastPOWBlock = 10000;          // Proof of Work finishes on block 10000

        genesis = CreateGenesisBlock(1473949500, 37239843, consensus.bnProofOfWorkLimit.GetCompact(), 1, (0 * COIN));

        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256("0x000000251356c62e0aa14c63e2b0db2a05ac0d3316ea5000c797a281be8c9fd7"));
        assert(genesis.hashMerkleRoot == uint256("0x73d6f8c42dfa8c9175b8bf4bf75ebfd10d22b0b6b1a39a82ce0e408447418e4b"));

        //vSeeds.push_back(CDNSSeedData("silknetwork.net", "slk1.silknetwork.net"));
        //vSeeds.push_back(CDNSSeedData("silknetwork.net", "slk2.silknetwork.net"));
        vSeeds.push_back(CDNSSeedData("dnsseeder.io", "silk.dnsseeder.io"));
        vSeeds.push_back(CDNSSeedData("dnsseeder.com", "silk.dnsseeder.com"));
        vSeeds.push_back(CDNSSeedData("dnsseeder.host", "silk.dnsseeder.host"));
        vSeeds.push_back(CDNSSeedData("dnsseeder.net", "silk.dnsseeder.net"));

        // Silk PUBKEY_ADDRESS addresses start with 'S'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,63);
        // Silk SCRIPT_ADDRESS addresses start with 'S or T'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,64);
        // Silk SECRET_KEY start with '2'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,170);
        // Silk BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // Silk BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        pchMessageStart[0] = 0x01;
        pchMessageStart[1] = 0x10;
        pchMessageStart[2] = 0x02;
        pchMessageStart[3] = 0x20;
        vAlertPubKey = ParseHex("04cd4cd0b961a17d0ca977580f123fc3836c3081491484f21c5e4b97d41d3762c30a51d1141b81c21dd6a23c55d4424a98b38a1e914c8158645ce51869e52b99b0");
        nDefaultPort = 16662;
        nMaxTipAge = 24 * 60 * 60;

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256("0x000000251356c62e0aa14c63e2b0db2a05ac0d3316ea5000c797a281be8c9fd7"))
            ( 200, uint256("0x000000297583f342ee3a945bfe4b7c9e30965cbfe97d0573d4a8ad3dfbd59c0c"))
            ( 1000, uint256("0x0000000cb3d1d4589f7f4b6339ca7106c3539146ad307a80232ab09a6f6140f1"))
            ( 4000, uint256("0x00000030b6226239a0d3b809e3b076e3a75476fe8048dc41859a6138f63a7a28"))
            ( 10000, uint256("0x0a7589f8bdc5e49f55e4ba3ba8875b909e7ca4802a0505b94d0b42b5f55d1598"))
            ( 40000, uint256("0xc6b43d4102098d0babf3529ebe9fc772bec026a36319b534d94f6fde64b963d9"))
            ( 160000, uint256("0x62cf48b78e93ef09d60c83da6da1c7b3dfa6602126e36d6756706124d2fb730b"))
            ( 200000, uint256("0xf6c0823bdede95bf0819929aa7ee5df69abccbde96a36211694312af84516b75")),
             1486676814, // * UNIX timestamp of last checkpoint block
            0,          // * total number of transactions between genesis and last checkpoint
            //   (the tx=... number in the SetBestChain debug.log lines)
            2000        // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.bnProofOfWorkLimit = ~uint256(0) >> 26;
        consensus.bnProofOfStakeLimit = ~uint256(0) >> 26;
        consensus.nEnforceBlockUpgradeMajority = 51;
        consensus.nRejectBlockOutdatedMajority = 75;
        consensus.nToCheckBlockUpgradeMajority = 100;
        consensus.nMinerThreads = 0;
        consensus.fAllowMinDifficultyBlocks = false;
        consensus.nStakeMinAge = 30 * 60;         // 30 minute minimum stake age
        consensus.nModifierInterval = 15 * 60;    // 15 minutes to elapse before new modifier is computed
        consensus.nLastPOWBlock = 100000;         // Proof of Work finishes on block 300000
        bool startNewChain = false;

        genesis = CreateTestNetGenesisBlock(1478107000, 82131309, consensus.bnProofOfWorkLimit.GetCompact(), 1, (0 * COIN));
        if(startNewChain == true) { MineGenesis(genesis, consensus.bnProofOfWorkLimit); }
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256("0x00000009cfa952b4f748c8cf7cdf975f89bce0a26a6a7e7c8cb33968bc20848e"));
        assert(genesis.hashMerkleRoot == uint256("0x2daaebb4a85d64d0a5266ca58656c8db03f95a7c9c863eb34872b48c3c6f3dea"));

        vFixedSeeds.clear();
        vSeeds.clear();

        // Silk PUBKEY_ADDRESS addresses start with 'N'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,53);
        // Silk PUBKEY_ADDRESS addresses start with 'M'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,50);
        // Silk SECRET_KEY start with 'X or 7'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,204);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x11)(0x35)(0xAA)(0xEE).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x35)(0x11)(0xDD)(0xFF).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        pchMessageStart[0] = 0x02;
        pchMessageStart[1] = 0x20;
        pchMessageStart[2] = 0x03;
        pchMessageStart[3] = 0x30;
        vAlertPubKey = ParseHex("");
        nDefaultPort = 16664;
        nMaxTipAge = 24 * 60 * 60;

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        
        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256("0x00000009cfa952b4f748c8cf7cdf975f89bce0a26a6a7e7c8cb33968bc20848e")),
            1478107000, // * UNIX timestamp of last checkpoint block
            0,          // * total number of transactions between genesis and last checkpoint
            //   (the tx=... number in the SetBestChain debug.log lines)
            2000        // * estimated number of transactions per day after checkpoint
        };
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nEnforceBlockUpgradeMajority = 750;
        consensus.nRejectBlockOutdatedMajority = 950;
        consensus.nToCheckBlockUpgradeMajority = 1000;
        consensus.nMinerThreads = 1;
        consensus.bnProofOfWorkLimit = ~uint256(0) >> 22;
        consensus.bnProofOfStakeLimit = ~uint256(0) >> 22;
        consensus.fAllowMinDifficultyBlocks = true;
        consensus.nLastPOWBlock = 100;    // Proof of Work finishes on block 100
        
        genesis = CreateGenesisBlock(1473949500, 1427578, 0x1e00ffff, 1, (0 * COIN));
        
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256("0x000003b8d718c47f10afdaa9b59c7d709e0bc8daf79b41dada0c411e7dc9985f"));
        assert(genesis.hashMerkleRoot == uint256("0x73d6f8c42dfa8c9175b8bf4bf75ebfd10d22b0b6b1a39a82ce0e408447418e4b"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        pchMessageStart[0] = 0x40;
        pchMessageStart[1] = 0x04;
        pchMessageStart[2] = 0x50;
        pchMessageStart[3] = 0x05;
        nDefaultPort = 16666;
        nMaxTipAge = 24 * 60 * 60;

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        
        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256("0x000003b8d718c47f10afdaa9b59c7d709e0bc8daf79b41dada0c411e7dc9985f")),
            1473949500, // * UNIX timestamp of last checkpoint block
            0,          // * total number of transactions between genesis and last checkpoint
            //   (the tx=... number in the SetBestChain debug.log lines)
            2000        // * estimated number of transactions per day after checkpoint
        };
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
        return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
        return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
