// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cosmos/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

using namespace TW;
using namespace TW::Cosmos;

TEST(OdinAddress, Valid) {
    EXPECT_TRUE(Address::isValid(TWCoinTypeOdin, "odin1fzjnsuwzxznqd3qlkt5njd4vvpcjqca6zckfux"));
    EXPECT_TRUE(Address::isValid(TWCoinTypeOdin, "odin10x79vuvw8adqfnvukda6j2vr8z238484nzazs7"));
}

TEST(OdinAddress, Invalid) {
    EXPECT_FALSE(Address::isValid(TWCoinTypeOdin, "odin1fzjnruwzxznqd3qlkt5njd4vvpcjqca6zckfux"));
    EXPECT_FALSE(Address::isValid(TWCoinTypeOdin,
                                  "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02")); // valid cosmos
}

TEST(OdinAddress, FromPrivateKey) {
    auto privateKey =
        PrivateKey(parse_hex("83e68457212d6026ec2fe18cf1ecb1ed2cb2aa8ecdec38d884c89c40dfb68f62"));
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Address(TWCoinTypeOdin, publicKey);
    ASSERT_EQ(address.string(), "odin1d224epchv57ftr6qwq98nzllfw9mmtzpfg4zla");
}

TEST(OdinAddress, FromPublicKey) {
    auto publicKey =
        PublicKey(parse_hex("028edd997cd83ecc9f82c70483d116de363efa9ee5ba6888e8483b8c7c6679fc29"),
                  TWPublicKeyTypeSECP256k1);
    auto address = Address(TWCoinTypeOdin, publicKey);
    ASSERT_EQ(address.string(), "odin1d224epchv57ftr6qwq98nzllfw9mmtzpfg4zla");
}

TEST(OdinAddress, FromString) {
    Address address;
    EXPECT_TRUE(Address::decode("odin1d224epchv57ftr6qwq98nzllfw9mmtzpfg4zla", address));
    EXPECT_EQ(address.string(), "odin1d224epchv57ftr6qwq98nzllfw9mmtzpfg4zla");
    EXPECT_EQ(hex(address.getKeyHash()), "6a955c8717653c958f40700a798bff4b8bbdac41");
}
