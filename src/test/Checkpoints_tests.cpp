// Copyright (c) 2009-2018 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Developers
// Copyright (c) 2016-2018 Duality Blockchain Solutions Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Unit tests for block-chain checkpoints
//

#include "checkpoints.h"

#include "uint256.h"

#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(Checkpoints_tests)

BOOST_AUTO_TEST_CASE(sanity)
{
    uint256 p25000 = uint256("0x20cc6639e9593e4e9344e1d40a234c552da81cb90b991aed6200ff0f72a69719");
    uint256 p100000 = uint256("0x0000000000000071c614fefb88072459cced7b9d9a9cffd04064d3c3d539ecaf");
    BOOST_CHECK(Checkpoints::CheckBlock(25000, p25000));
    BOOST_CHECK(Checkpoints::CheckBlock(100000, p100000));

    
    // Wrong hashes at checkpoints should fail:
    BOOST_CHECK(!Checkpoints::CheckBlock(25000, p100000));
    BOOST_CHECK(!Checkpoints::CheckBlock(100000, p25000));

    // ... but any hash not at a checkpoint should succeed:
    BOOST_CHECK(Checkpoints::CheckBlock(25000+1, p100000));
    BOOST_CHECK(Checkpoints::CheckBlock(100000+1, p25000));

    BOOST_CHECK(Checkpoints::GetTotalBlocksEstimate() >= 100000);
}    

BOOST_AUTO_TEST_SUITE_END()
