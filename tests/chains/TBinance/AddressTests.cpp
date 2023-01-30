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
    ASSERT_TRUE(TAddress::isValid("tbnb18mtcq20875cr0p7l4upm0u5zx4r9jpj2kfu9f8"));
}

TEST(TBinanceAddress, Invalid) {
    ASSERT_FALSE(TAddress::isValid("tbnb18mtcq20875cr0p7l4upm0u5zx4r9jpj2kfu9aa"));
}

TEST(TBinanceAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("fc75070f2d9939be82a378ec9a47912cb6df458055f51da022021f42a6bb5ee8"));
    auto address = TAddress(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    ASSERT_EQ(address.string(), "tbnb18mtcq20875cr0p7l4upm0u5zx4r9jpj2kfu9f8");
}

TEST(TBinanceAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("02d32c10f9f4b72d0213123d58257e0558e164e5373e719aee73ce5505852c1692"), TWPublicKeyTypeSECP256k1);
    auto address = TAddress(publicKey);
    ASSERT_EQ(address.string(), "tbnb18mtcq20875cr0p7l4upm0u5zx4r9jpj2kfu9f8");
}

TEST(TBinanceAddress, FromString) {
    TAddress address;
    TAddress::decode("tbnb18mtcq20875cr0p7l4upm0u5zx4r9jpj2kfu9f8", address);
    ASSERT_EQ(address.string(), "tbnb18mtcq20875cr0p7l4upm0u5zx4r9jpj2kfu9f8");
}
