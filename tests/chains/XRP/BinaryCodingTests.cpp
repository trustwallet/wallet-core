// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
