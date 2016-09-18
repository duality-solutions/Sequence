// Copyright (c) 2009-2016 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Developers
// Copyright (c) 2015-2016 Silk Network Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SILK_RPCCLIENT_H
#define SILK_RPCCLIENT_H

#include <univalue.h>

UniValue RPCConvertValues(const std::string& strMethod, const std::vector<std::string>& strParams);

/** Non-RFC4627 JSON parser, accepts internal values (such as numbers, true, false, null)
 * as well as objects and arrays.
 */
UniValue ParseNonRFCJSONValue(const std::string& strVal);

#endif // SILK_RPCCLIENT_H
