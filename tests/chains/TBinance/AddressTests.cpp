// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Binance/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Binance;

TEST(TBinanceAddress, Valid) {
    ASSERT_TRUE(Address::isValid(TWCoinTypeTBinance, "tbnb18mtcq20875cr0p7l4upm0u5zx4r9jpj2kfu9f8"));
}

TEST(TBinanceAddress, Invalid) {
    ASSERT_FALSE(Address::isValid(TWCoinTypeTBinance, "tbnb18mtcq20875cr0p7l4upm0u5zx4r9jpj2kfu9aa"));
    ASSERT_FALSE(Address::isValid(TWCoinTypeTBinance, "bnb1h8xf9htasu9aclra954dnnve8fgcda4ae7qfa8"));
}

TEST(TBinanceAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("fc75070f2d9939be82a378ec9a47912cb6df458055f51da022021f42a6bb5ee8"));
    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1), TWCoinTypeTBinance);
    ASSERT_EQ(address.string(), "tbnb18mtcq20875cr0p7l4upm0u5zx4r9jpj2kfu9f8");
}

TEST(TBinanceAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("02d32c10f9f4b72d0213123d58257e0558e164e5373e719aee73ce5505852c1692"), TWPublicKeyTypeSECP256k1);
    auto address = Address(publicKey, TWCoinTypeTBinance);
    ASSERT_EQ(address.string(), "tbnb18mtcq20875cr0p7l4upm0u5zx4r9jpj2kfu9f8");
}

TEST(TBinanceAddress, FromString) {
    Address address(TWCoinTypeTBinance);
    Address::decode("tbnb18mtcq20875cr0p7l4upm0u5zx4r9jpj2kfu9f8", address);
    ASSERT_EQ(address.string(), "tbnb18mtcq20875cr0p7l4upm0u5zx4r9jpj2kfu9f8");
}
