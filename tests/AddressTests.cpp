// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include <gtest/gtest.h>

namespace TW {

TEST(Address, ValidateEthereum) {
    EXPECT_TRUE(validate("0xeDe8F58dADa22c3A49dB60D4f82BAD428ab65F89", TWCoinTypeEthereum));
    EXPECT_FALSE(validate("ede8f58dada22a49db60d4f82bad428ab65f89", TWCoinTypeEthereum));
}

TEST(Address, ValidateBitcoin) {
    EXPECT_TRUE(validate("bc1q2ddhp55sq2l4xnqhpdv0xazg02v9dr7uu8c2p2", TWCoinTypeBitcoin));
    EXPECT_TRUE(validate("1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2", TWCoinTypeBitcoin));
    EXPECT_TRUE(validate("3J98t1WpEZ73CNmQviecrnyiWrnqRhWNLy", TWCoinTypeBitcoin));

    EXPECT_FALSE(validate("bc1q2ddhp55sq2l4xnqhpdv9xazg02v9dr7uu8c2p2", TWCoinTypeBitcoin));
    EXPECT_FALSE(validate("MPmoY6RX3Y3HFjGEnFxyuLPCQdjvHwMEny", TWCoinTypeBitcoin));
}

TEST(Address, ValidateLitecoin) {
    EXPECT_TRUE(validate("ltc1q5wmm9vrz55war9c0rgw26tv9un5fxnn7slyjpy", TWCoinTypeLitecoin));
    EXPECT_TRUE(validate("MPmoY6RX3Y3HFjGEnFxyuLPCQdjvHwMEny", TWCoinTypeLitecoin));

    EXPECT_FALSE(validate("1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2", TWCoinTypeLitecoin));
}

} // namespace
