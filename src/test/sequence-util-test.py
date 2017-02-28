# Copyright (c) 2009-2017 Satoshi Nakamoto
# Copyright (c) 2009-2017 The Bitcoin Developers
# Copyright (c) 2013-2017 Emercoin Developers
# Copyright (c) 2016-2017 Duality Blockchain Solutions Developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

import os
import bctest
import buildenv

if __name__ == '__main__':
	bctest.bctester(os.environ["srcdir"] + "/test/data",
			"sequence-util-test.json",buildenv)

