// Copyright (c) 2009-2017 Satoshi Nakamoto
// Copyright (c) 2016-2017 Presstab & SSTA
// Copyright (c) 2009-2017 The Bitcoin Developers
// Copyright (c) 2015-2017 The Sequence Developers
// Copyright (c) 2016-2017 Duality Blockchain Solutions Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "wallet/wallet.h"
#include "wallet/walletdb.h"
#include "rpcserver.h"
#include "main.h"
#include "init.h"
#include "base58.h"
#include "coincontrol.h"

#include <sstream>
#include <boost/lexical_cast.hpp>

using namespace std;

CCoinControl* coinControl = new CCoinControl;

double GetMoneySupply(int nHeight)
{
	CBlockIndex* pindex = chainActive[nHeight];
	double nSupply = pindex->nMoneySupply;	
	return nSupply / COIN;	
}

double GetSupplyChange(int nHeight, int pHeight)
{
	double nSupply = GetMoneySupply(nHeight); //present supply
	double pSupply = GetMoneySupply(pHeight); //previous supply
	double nChange = nSupply - pSupply; //difference
	return nChange;
}

double GetBlockSpeed(int nHeight, int pHeight)
{
	CBlockIndex* pIndex = chainActive[nHeight];
	CBlockIndex* ppIndex = chainActive[pHeight];
	double nTime = pIndex->nTime;
	double pTime = ppIndex->nTime;
	double nTimeChange = (nTime - pTime) / 60 / 60 / 24; //in days
	return nTimeChange;
}

double GetRate(int nHeight, int pHeight)
{
	double nSupplyChange = GetSupplyChange(nHeight, pHeight);
	double nTimeChange = GetBlockSpeed(nHeight, pHeight);
	double nMoneySupply = GetMoneySupply(nHeight);
	double nRate = nSupplyChange / nMoneySupply / nTimeChange;
	
	return nRate;
}

UniValue detailedmoneysupply(const UniValue & params, bool fHelp)
{
	if (fHelp || params.size() != 0)
        throw std::runtime_error(
            "moneysupply\n"
            "Show important money supply variables.\n");
	
	const CBlockIndex* blockindex = GetLastBlockIndex(chainActive.Tip(), false);

	int64_t nHeight = blockindex->nHeight;
	int64_t n1Height = nHeight - 1440;
	int64_t n7Height = nHeight - 1440 * 7;
	int64_t n30Height = nHeight - 1440 * 30;
	
	UniValue obj(UniValue::VARR);
	
	obj.push_back(Pair("moneysupply - present", GetMoneySupply(nHeight)));
	obj.push_back(Pair("moneysupply - 1,440 blocks ago", GetMoneySupply(n1Height)));
	obj.push_back(Pair("moneysupply - 10,080 blocks ago", GetMoneySupply(n7Height)));
	obj.push_back(Pair("moneysupply - 43,200 blocks ago", GetMoneySupply(n30Height)));
	
	obj.push_back(Pair("supply change(last 1,440 blocks)", GetSupplyChange(nHeight, n1Height)));
	obj.push_back(Pair("supply change(last 10,080 blocks)", GetSupplyChange(nHeight, n7Height)));
	obj.push_back(Pair("supply change(last 43,200 blocks)", GetSupplyChange(nHeight, n30Height)));
	
	obj.push_back(Pair("time change over 1,440 blocks", GetBlockSpeed(nHeight, n1Height)));
	obj.push_back(Pair("time change over 10,080 blocks", GetBlockSpeed(nHeight, n7Height)));
	obj.push_back(Pair("time change over 43,200 blocks", GetBlockSpeed(nHeight, n30Height)));
	
	obj.push_back(Pair("avg daily rate of change (last 1,440 blocks)", GetRate(nHeight, n1Height)));
	obj.push_back(Pair("avg daily rate of change (last 10,080 blocks)", GetRate(nHeight, n7Height)));
	obj.push_back(Pair("avg daily rate of change (last 43,200 blocks)", GetRate(nHeight, n30Height)));
	
	return obj;
}


UniValue cclistcoins(const UniValue & params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw std::runtime_error(
            "cclistcoins\n"
			"CoinControl: list your spendable coins and their information\n");
	
	UniValue result(UniValue::VARR);
	
	std::vector<COutput> vCoins;
    pwalletMain->AvailableCoins(vCoins);
	
	for (const COutput& out : vCoins)
    {
		UniValue coutput(UniValue::VOBJ);
		
		int64_t nHeight = chainActive.Height() - out.nDepth;
		CBlockIndex* pindex = chainActive[nHeight];
		
		CTxDestination outputAddress;
		ExtractDestination(out.tx->vout[out.i].scriptPubKey, outputAddress);
		coutput.push_back(Pair("Address", CSequenceAddress(outputAddress).ToString()));
		coutput.push_back(Pair("Output Hash", out.tx->GetHash().ToString()));
		coutput.push_back(Pair("blockIndex", out.i));
		double dAmount = double(out.tx->vout[out.i].nValue) / double(COIN);
		coutput.push_back(Pair("Value", dAmount));
		coutput.push_back(Pair("Confirmations", int(out.nDepth)));
		double dAge = double(GetTime() - pindex->nTime);
		coutput.push_back(Pair("Age (days)", (dAge/(60*60*24))));
		double nReward = (STATIC_POS_REWARD/COIN) / 365 * dAge * dAmount;
		nReward = min(nReward, double(30));
		coutput.push_back(Pair("Potential Stake", nReward));
		
		result.push_back(coutput);
	}
	
	return result;
}

UniValue ccselect(const UniValue & params, bool fHelp)
{
    if (fHelp || params.size() != 2)
        throw std::runtime_error(
            "ccselect <Output Hash> <Output Index>\n"
			"CoinControl: select a coin");
	
	uint256 hash;
    hash.SetHex(params[0].get_str());
	unsigned int nIndex = params[1].get_int();
	COutPoint outpt(hash, nIndex);
	coinControl->Select(outpt);

	return "Outpoint Selected";
}

UniValue cclistselected(const UniValue & params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw std::runtime_error(
            "cclistselected\n"
			"CoinControl: list selected coins");
	
	std::vector<COutPoint> vOutpoints;
	coinControl->ListSelected(vOutpoints);
	
	UniValue result(UniValue::VARR);
	
	for (COutPoint& outpt : vOutpoints)
	{
		result.push_back(outpt.hash.ToString());
	}

	return result;
}

UniValue ccreturnchange(const UniValue & params, bool fHelp)
{
    if (fHelp || params.size() != 1)
        throw std::runtime_error(
            "ccreturnchange <true|false>\n"
                        "CoinControl: sets returnchange to true or false");
    bool rc = params[0].get_bool();
    coinControl->fReturnChange=rc;
    string ret = "Set ReturnChange to: ";
    
	if(coinControl->fReturnChange )
		ret+= "true";
	else
		ret+= "false";
		
    return ret;
}

UniValue cccustomchange(const UniValue & params, bool fHelp)
{
    if (fHelp || params.size() != 1)
        throw std::runtime_error(
            "cccustomchange <address>\n"
                        "CoinControl: sets address to return change to");
    CSequenceAddress address(params[0].get_str());
    // check it's a valid address
    if(!address.IsValid()) throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid Sequence address");

    coinControl->destChange=address.Get();

    string ret = "Set change address to: ";
    ret+=(CSequenceAddress(coinControl->destChange).ToString());
    return ret;
}

UniValue ccreset(const UniValue & params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw std::runtime_error(
            "ccreset\n"
                        "CoinControl: resets coin control (clears selected coins and change address)");
    coinControl->SetNull();
    return NullUniValue;
}

UniValue ccsend(const UniValue & params, bool fHelp)
{
    if (fHelp || params.size() != 2)
        throw std::runtime_error(
		"ccsend <Sequence Address> <amount>\n"
            "<amount> is a real and is rounded to the nearest 0.000001"
            + HelpRequiringPassphrase());

    CSequenceAddress address(params[0].get_str());
    if (!address.IsValid())
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid Sequence address");

    // Amount
    int64_t nAmount = AmountFromValue(params[1]);

    if (nAmount < DEFAULT_TRANSACTION_FEE + 1)
        throw JSONRPCError(-101, "Send amount too small");

    if (pwalletMain->IsLocked())
        throw JSONRPCError(RPC_WALLET_UNLOCK_NEEDED, "Error: Please enter the wallet passphrase with walletpassphrase first.");

	// Initialize things needed for the transaction
	vector<pair<CScript, int64_t> > vecSend;
	CWalletTx wtx;
    CReserveKey keyChange(pwalletMain);
    int64_t nFeeRequired = 0;
	CScript scriptPubKey = GetScriptForDestination(address.Get());
    vecSend.push_back(make_pair(scriptPubKey, nAmount));
	std::string strFailReason;
    bool fCreated = pwalletMain->CreateTransaction(vecSend, wtx, keyChange, nFeeRequired, 1, strFailReason, coinControl); // 1 = no splitblock, false for s4c, coinControl
    if (!fCreated)
    {
        if (nAmount + nFeeRequired > pwalletMain->GetBalance())
            throw JSONRPCError(RPC_WALLET_INSUFFICIENT_FUNDS, "Insufficient funds");
        throw JSONRPCError(RPC_WALLET_ERROR, strFailReason);
    }
    if (!pwalletMain->CommitTransaction(wtx, keyChange))
        throw JSONRPCError(RPC_WALLET_ERROR, "Transaction commit failed");
	
	coinControl->SetNull();
    return wtx.GetHash().GetHex();
}

/*
 * TODO: FIX ME! 
 */

UniValue getstaketx(const UniValue & params, bool fHelp)
{
    if (fHelp || params.size() != 1)
        throw std::runtime_error(
            "getstaketx <txid>\n"
            "Get detailed information about a specific stake <txid>");

    uint256 hash;
    hash.SetHex(params[0].get_str());

    UniValue entry(UniValue::VARR), vin(UniValue::VARR);

    if (pwalletMain->mapWallet.count(hash))
    {
        const CWalletTx& wtx = pwalletMain->mapWallet[hash];
		
		for (const CTxIn& txin : wtx.vin)
		{
			UniValue in(UniValue::VOBJ);
			if (wtx.IsCoinBase())
				entry.push_back(Pair("coinbase", HexStr(txin.scriptSig.begin(), txin.scriptSig.end())));
			else
			{
				CTransaction& txPrev = pwalletMain->mapWallet[txin.prevout.hash]; //first transaction
				uint64_t nTime = wtx.nTime; //stake tx time
				uint64_t nPrevTime = txPrev.nTime; //previous tx time
				uint64_t nTimeToStake = nTime - nPrevTime; // time to stake in seconds
				double dDaysToStake = nTimeToStake / 60.00 / 60 / 24;
				
				int64_t nDebit = wtx.GetDebit(ISMINE_SPENDABLE);
				int64_t nFee = (wtx.IsFromMe(ISMINE_SPENDABLE) ? wtx.GetValueOut() - nDebit : 0);
				int64_t /* nGeneratedImmature, nGeneratedMature, */ nFee2;
				string strSentAccount;
                std::list<COutputEntry> listReceived;
                std::list<COutputEntry> listSent;
				wtx.GetAmounts(listReceived, listSent, nFee2, strSentAccount, ISMINE_SPENDABLE);
				/* uint64_t nGeneratedAmount = max (nGeneratedMature, nGeneratedImmature);
				double nGeneratedAmount2 = max (nGeneratedMature, nGeneratedImmature); //uint64_t math not working
				double percentReward = nFee / (nGeneratedAmount2 - nFee);
				*/
				entry.push_back(Pair("Stake TX Time", nTime));
				entry.push_back(Pair("Previous Time", nPrevTime));
				entry.push_back(Pair("Days To Stake", dDaysToStake));
				// entry.push_back(Pair("Original Amount", ValueFromAmount(nGeneratedAmount - nFee)));
				entry.push_back(Pair("PoS Reward", ValueFromAmount(nFee)));
				// entry.push_back(Pair("Reward %", percentReward));
				// entry.push_back(Pair("Total New Amount", ValueFromAmount(nGeneratedAmount)));
				// entry.push_back(Pair("Size of Each New Block", ValueFromAmount(nGeneratedAmount/2)));
			}
		}
    }
    return entry;
}

UniValue setstakesplitthreshold(const UniValue & params, bool fHelp)
{
    if (fHelp || params.size() != 1)
        throw std::runtime_error(
            "setstakesplitthreshold <1 - 999,999>\n"
            "This will set the output size of your stakes to never be below this number\n");
    
	uint64_t nStakeSplitThreshold = boost::lexical_cast<int>(params[0].get_str());
	if (pwalletMain->IsLocked())
        throw JSONRPCError(RPC_WALLET_UNLOCK_NEEDED, "Error: Unlock wallet to use this feature");
	if (nStakeSplitThreshold > 999999)
		return "out of range - setting split threshold failed";
	
	CWalletDB walletdb(pwalletMain->strWalletFile);
	LOCK(pwalletMain->cs_wallet);
	{
		bool fFileBacked = pwalletMain->fFileBacked;
		
		UniValue result(UniValue::VARR);
		pwalletMain->nStakeSplitThreshold = nStakeSplitThreshold;
		result.push_back(Pair("split stake threshold set to ", int(pwalletMain->nStakeSplitThreshold)));
		if(fFileBacked)
		{
			walletdb.WriteStakeSplitThreshold(nStakeSplitThreshold);
			result.push_back(Pair("saved to wallet.dat ", "true"));
		}
		else
			result.push_back(Pair("saved to wallet.dat ", "false"));
		
		return result;
	}
}

UniValue getstakesplitthreshold(const UniValue & params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw std::runtime_error(
            "getstakesplitthreshold\n"
            "Returns the set splitstakethreshold\n");

	UniValue result(UniValue::VARR);
	result.push_back(Pair("split stake threshold set to ", int(pwalletMain->nStakeSplitThreshold)));
	
	return result;
}
