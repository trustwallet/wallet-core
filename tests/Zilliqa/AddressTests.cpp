// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "Zilliqa/Address.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Zilliqa;

TEST(ZilliqaAddress, FromPrivateKey) {
    const auto privateKey =
        PrivateKey(parse_hex("3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6"));
    const auto publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const auto address = Address(publicKey);
    auto expectedAddress = "0x91cddcebe846ce4d47712287eee53cf17c2cfb77";
    ASSERT_EQ(address.string(), expectedAddress);
}

TEST(ZilliqaAddress, fromBadString) {
    EXPECT_ANY_THROW(Address("0x3"));
}

TEST(ZilliqaAddress, fromBadPubKey) {
    const auto privateKey =
        PrivateKey(parse_hex("3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6"));
    PublicKey publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeED25519));
    EXPECT_ANY_THROW((Address(publicKey)));
}

TEST(ZilliqaAddress, fromString) {
    const auto expected = "0x91cddcebe846ce4d47712287eee53cf17c2cfb77";
    ASSERT_EQ(Address(expected).string(), expected);
}

TEST(ZilliqaAddress, IsValid) {
    ASSERT_FALSE(Address::isValid("0x91cddcebe846ce4d47712287eee53cf17c2cfb7"));
    ASSERT_FALSE(Address::isValid(""));
    ASSERT_FALSE(Address::isValid("0x"));
    ASSERT_FALSE(Address::isValid("91cddcebe846ce4d47712287eee53cf17c2cfb7"));
    ASSERT_TRUE(Address::isValid("0x91cddcebe846ce4d47712287eee53cf17c2cfb77"));
}
