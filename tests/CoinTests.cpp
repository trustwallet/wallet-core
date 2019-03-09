// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW {

TEST(Coin, ValidateAddressEthereum) {
    EXPECT_TRUE(validateAddress(TWCoinTypeEthereum, "0xeDe8F58dADa22c3A49dB60D4f82BAD428ab65F89"));
    EXPECT_FALSE(validateAddress(TWCoinTypeEthereum, "ede8f58dada22a49db60d4f82bad428ab65f89"));
}

TEST(Coin, DeriveAddressEthereum) {
    const auto pk = PrivateKey(parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646"));
    EXPECT_EQ(deriveAddress(TWCoinTypeEthereum, pk), "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F");
}

TEST(Coin, validateAddressBitcoin) {
    EXPECT_TRUE(validateAddress(TWCoinTypeBitcoin, "bc1q2ddhp55sq2l4xnqhpdv0xazg02v9dr7uu8c2p2"));
    EXPECT_TRUE(validateAddress(TWCoinTypeBitcoin, "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2"));
    EXPECT_TRUE(validateAddress(TWCoinTypeBitcoin, "3J98t1WpEZ73CNmQviecrnyiWrnqRhWNLy"));

    EXPECT_FALSE(validateAddress(TWCoinTypeBitcoin, "bc1q2ddhp55sq2l4xnqhpdv9xazg02v9dr7uu8c2p2"));
    EXPECT_FALSE(validateAddress(TWCoinTypeBitcoin, "MPmoY6RX3Y3HFjGEnFxyuLPCQdjvHwMEny"));
}

TEST(Coin, DeriveAddressBitcoin) {
    const auto pk = PrivateKey(parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646"));
    EXPECT_EQ(deriveAddress(TWCoinTypeBitcoin, pk), "bc1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z00ppggv");
}

TEST(Coin, ValidateAddressLitecoin) {
    EXPECT_TRUE(validateAddress(TWCoinTypeLitecoin, "ltc1q5wmm9vrz55war9c0rgw26tv9un5fxnn7slyjpy"));
    EXPECT_TRUE(validateAddress(TWCoinTypeLitecoin, "MPmoY6RX3Y3HFjGEnFxyuLPCQdjvHwMEny"));

    EXPECT_FALSE(validateAddress(TWCoinTypeLitecoin, "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2"));
}

TEST(Coin, ValidateAddressBitcoinCash) {
    EXPECT_TRUE(validateAddress(TWCoinTypeBitcoinCash, "bitcoincash:qruxj7zq6yzpdx8dld0e9hfvt7u47zrw9gfr5hy0vh"));
    EXPECT_TRUE(validateAddress(TWCoinTypeBitcoinCash, "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2"));
}

TEST(Coin, ValidateAddressTezos) {
  EXPECT_TRUE(validateAddress(TWCoinTypeTezos, "tz1d1qQL3mYVuiH4JPFvuikEpFwaDm85oabM"));

  EXPECT_FALSE(validateAddress(TWCoinTypeTezos, "NmH7tmeJUmHcncBDvpr7aJNEBk7rp5zYsB1qt"));
  EXPECT_FALSE(validateAddress(TWCoinTypeTezos, "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3AAAA"));
  EXPECT_FALSE(validateAddress(TWCoinTypeTezos, "1tzeZwq8b5cvE2bPKokatLkVMzkxz24zAAAAA"));
}

} // namespace
