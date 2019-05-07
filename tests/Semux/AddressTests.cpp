// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Semux/Address.h"

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Semux;

auto publicKeyHex = "b72dc8ebc9f53d21837dc96483da08765ea11f25c1bd4c3cb49318c944d67b9b";
auto privateKeyHex = "bd2f24b259aac4bfce3792c31d0f62a7f28b439c3e4feb97050efe5fe254f2af";
auto addressHex = "0x0680a919c78faa59b127014b6181979ae0a62dbd";

TEST(SemuxAddress, FromPublicKey) {
    const auto publicKey = PublicKey(parse_hex(publicKeyHex), TWPublicKeyTypeED25519);
    const auto address = Address(publicKey);
    ASSERT_EQ(address.string(), addressHex);
}

TEST(SemuxAddress, FromString) {
    ASSERT_EQ(Address(addressHex).string(), addressHex);
}

TEST(SemuxAddress, FromBadPubKey) {
    const auto privateKey =
        PrivateKey(parse_hex("3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6"));
    PublicKey publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    EXPECT_ANY_THROW((Address(publicKey)));
}

TEST(SemuxAddress, FromBadString) {
    EXPECT_ANY_THROW(Address("0x3"));
}

TEST(SemuxAddress, IsValid) {
    ASSERT_FALSE(Address::isValid("0x91cddcebe846ce4d47712287eee53cf17c2cfb7"));
    ASSERT_FALSE(Address::isValid(""));
    ASSERT_FALSE(Address::isValid("0x"));
    ASSERT_FALSE(Address::isValid("91cddcebe846ce4d47712287eee53cf17c2cfb7"));
    ASSERT_TRUE(Address::isValid(addressHex));
}

TEST(SemuxAddress, FromPrivateKey) {
    const auto privateKey = PrivateKey(parse_hex(privateKeyHex));
    const auto publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeED25519));
    const auto address = Address(publicKey);
    ASSERT_EQ(address.string(), addressHex);
}