// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "UInt.h"
#include "uint256.h"
#include "Data.h"

#include <gtest/gtest.h>

namespace TW {

TEST(UInt, UInt256_To_Data) {
    EXPECT_EQ(store(uint256_t(0)), Data({0}));
    EXPECT_EQ(store(uint256_t(2)), Data({2}));
    EXPECT_EQ(store(uint256_t(3)), Data({3}));
    EXPECT_EQ(store(uint256_t(392)), Data({1, 136}));
    EXPECT_EQ(store(uint256_t(67976)), Data({1, 9, 136}));
    EXPECT_EQ(store(uint256_t(33622408)), Data({2, 1, 9, 136}));
}

TEST(UInt, Data_To_Hex) {
    EXPECT_EQ(hex(Data({2, 1, 9, 136})), "02010988");
    EXPECT_EQ(hex(Data({2, 1, 10, 136})), "02010a88");
}

TEST(UInt, Hex_To_Data) {
    EXPECT_EQ(Data({2, 1, 9, 136}), parse_hex("02010988"));
    EXPECT_EQ(Data({2, 1, 10, 136}), parse_hex("02010a88"));
}

} // namespace TW
