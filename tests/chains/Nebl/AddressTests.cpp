// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "Bitcoin/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

#include <TrustWalletCore/TWCoinType.h>

using namespace TW;
using namespace TW::Bitcoin;

TEST(NeblAddress, Valid) {
    ASSERT_TRUE(Address::isValid(std::string("NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc")));
    ASSERT_TRUE(TW::validateAddress(TWCoinTypeNebl, "NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc"));
}

TEST(NeblAddress, Invalid) {
    ASSERT_FALSE(Address::isValid(std::string("TZzJsL7VSUVeTcLb12LHr5tUW9bcx1T4G3")));
}

TEST(NeblAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("4222aae79af41eade7b07ce6fd44d926ea8e3f95e51a06e85f8bdec89680cbd9"));
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Address(publicKey, TWCoinTypeP2pkhPrefix(TWCoinTypeNebl));
    ASSERT_EQ(address.string(), "NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc");
}

TEST(NeblAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("03787a4c5ff72dce6d97f9b6360dc302b2d8a833e8c570dcc124a96e5f564bb524"), TWPublicKeyTypeSECP256k1);
    auto address = Address(publicKey, TWCoinTypeP2pkhPrefix(TWCoinTypeNebl));
    ASSERT_EQ(address.string(), "NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc");

    auto addr = TW::deriveAddress(TWCoinTypeNebl, publicKey, TWDerivationBitcoinLegacy);
    ASSERT_EQ(addr, "NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc");
}

TEST(NeblAddress, FromString) {
    auto address = Address("NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc");
    ASSERT_EQ(address.string(), "NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc");

    auto data = TW::addressToData(TWCoinTypeNebl, "NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc");
    EXPECT_EQ(hex(data), "ae40b2142aba5ddd10f74d9440bfda8a36cbad5b");

    // invalid address
    data = TW::addressToData(TWCoinTypeNebl, "xZW297yHHzSdiiwqE1eN4bfm5tJULjVZ");
    EXPECT_EQ(data.size(), 0ul);
}
