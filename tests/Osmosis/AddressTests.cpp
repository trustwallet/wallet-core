// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cosmos/Address.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Cosmos;

TEST(OsmosisAddress, Valid) {
    EXPECT_TRUE(Address::isValid(TWCoinTypeOsmosis, "osmo1mky69cn8ektwy0845vec9upsdphktxt0en97f5"));
    EXPECT_TRUE(Address::isValid(TWCoinTypeOsmosis, "osmo18s0hdnsllgcclweu9aymw4ngktr2k0rkvn7jmn"));
}

TEST(OsmosisAddress, Invalid) {
    EXPECT_FALSE(Address::isValid(TWCoinTypeOsmosis, "osmo1mky69cn8ektwy0845vec9upsdphktxt0en97f6"));
    EXPECT_FALSE(Address::isValid(TWCoinTypeOsmosis, "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02")); // valid cosmos
}

TEST(OsmosisAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af"));
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Address(TWCoinTypeOsmosis, publicKey);
    ASSERT_EQ(address.string(), "osmo1mky69cn8ektwy0845vec9upsdphktxt0en97f5");
}

TEST(OsmosisAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("02ecef5ce437a302c67f95468de4b31f36e911f467d7e6a52b41c1e13e1d563649"), TWPublicKeyTypeSECP256k1);
    auto address = Address(TWCoinTypeOsmosis, publicKey);
    ASSERT_EQ(address.string(), "osmo1mky69cn8ektwy0845vec9upsdphktxt0en97f5");
}

TEST(OsmosisAddress, FromString) {
    Address address;
    EXPECT_TRUE(Address::decode("osmo1mky69cn8ektwy0845vec9upsdphktxt0en97f5", address));
    EXPECT_EQ(address.string(), "osmo1mky69cn8ektwy0845vec9upsdphktxt0en97f5");
    EXPECT_EQ(hex(address.getKeyHash()), "dd89a2e267cd96e23cf5a33382f030686f65996f");
}
