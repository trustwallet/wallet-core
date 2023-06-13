// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "XRP/BinaryCoding.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Ripple::tests {
using namespace std;

TEST(RippleBinaryCoding, encodeVariableLength) {
    Data data;
    encodeVariableLength(180, data);
    EXPECT_EQ(hex(data), "b4");

    data.clear();
    encodeVariableLength(12080, data);
    EXPECT_EQ(hex(data), "2e6f");

    data.clear();
    encodeVariableLength(12580, data);
    EXPECT_EQ(hex(data), "000063");
}

} // namespace TW::Ripple::tests
