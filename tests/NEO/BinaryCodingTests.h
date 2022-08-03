// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "NEO/BinaryCoding.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::NEO;

TEST(NEOBinaryCoding, encode256LE) {
    Data d;
    encode256LE(d, uint256_t(110000000));
    EXPECT_EQ(hex(d), "80778e06");
}

TEST(NEOBinaryCoding, encode256LEWithPadding) {
    Data d;
    encode256LE(d, uint256_t(10000000));
    EXPECT_EQ(hex(d), "80969800");
}