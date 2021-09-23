// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Cosmos/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Cosmos;

TEST(CryptoorgAddress, Valid) {
    ASSERT_TRUE(Address::isValid(TWCoinTypeCryptoOrg, "cro1ctwtcwpgksky988dhth6jslxveumgu0d45zgf0"));
    ASSERT_TRUE(Address::isValid(TWCoinTypeCryptoOrg, "cro1xpahy6c7wldxacv6ld99h435mhvfnsup24vcus"));
    ASSERT_TRUE(Address::isValid(TWCoinTypeCryptoOrg, "cro1z53wwe7md6cewz9sqwqzn0aavpaun0gw39h3rd"));
    ASSERT_TRUE(Address::isValid(TWCoinTypeCryptoOrg, "cro1y8ua5laceufhqtwzyhahq0qk7rm87hhugtsfey"));
}

TEST(CryptoorgAddress, Invalid) {
    EXPECT_FALSE(Address::isValid(TWCoinTypeCryptoOrg, "cro1ctwtcwpgksky988dhth6jslxveumgu0d45zgf1"));
    EXPECT_FALSE(Address::isValid(TWCoinTypeCryptoOrg, "cro1z53wwe7md6cewz9sqwqzn0aavpaun0gw39h3re"));
    EXPECT_FALSE(Address::isValid(TWCoinTypeCryptoOrg, "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02")); // valid cosmos
}

TEST(CryptoorgAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e"));
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Address(TWCoinTypeCryptoOrg, publicKey);
    EXPECT_EQ(address.string(), "cro1z53wwe7md6cewz9sqwqzn0aavpaun0gw39h3rd");
    EXPECT_EQ(hex(address.getKeyHash()), "1522e767db6eb19708b0038029bfbd607bc9bd0e");
}

TEST(CryptoorgAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("03ed997e396cf4292f5fce5a42bba41599ccd5d96e313154a7c9ea7049de317c77"), TWPublicKeyTypeSECP256k1);
    auto address = Address(TWCoinTypeCryptoOrg, publicKey);
    EXPECT_EQ(address.string(), "cro1z53wwe7md6cewz9sqwqzn0aavpaun0gw39h3rd");
    EXPECT_EQ(hex(address.getKeyHash()), "1522e767db6eb19708b0038029bfbd607bc9bd0e");
}

TEST(CryptoorgAddress, FromString) {
    Address address;
    EXPECT_TRUE(Address::decode("cro1ctwtcwpgksky988dhth6jslxveumgu0d45zgf0", address));
    EXPECT_EQ(address.string(), "cro1ctwtcwpgksky988dhth6jslxveumgu0d45zgf0");
    EXPECT_EQ(hex(address.getKeyHash()), "c2dcbc3828b42c429cedbaefa943e66679b471ed");
}
