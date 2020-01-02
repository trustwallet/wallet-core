// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <gtest/gtest.h>

TEST(DataTests, ParseHex) {
    auto zero = DATA("0x0");
    ASSERT_EQ(TWDataSize(zero.get()), 1);
    ASSERT_EQ(TWDataBytes(zero.get())[0], 0);

    auto num = DATA("0xdeadbeef");
    ASSERT_EQ(TWDataSize(num.get()), 4);
    ASSERT_EQ(TWDataBytes(num.get())[0], 0xde);
    ASSERT_EQ(TWDataBytes(num.get())[1], 0xad);
    ASSERT_EQ(TWDataBytes(num.get())[2], 0xbe);
    ASSERT_EQ(TWDataBytes(num.get())[3], 0xef);
}

TEST(DataTests, Equal) {
    auto data1 = DATA("0xdeadbeef");
    auto data2 = DATA("0xdeadbeef");

    ASSERT_TRUE(TWDataEqual(data1.get(), data2.get()));
}
