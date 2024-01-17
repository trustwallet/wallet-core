// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Cosmos/Address.h"

#include <gtest/gtest.h>
#include <TrustWalletCore/TWHDWallet.h>

namespace TW::Cosmos {

TEST(CosmosAddressAddressToData, Invalid) {
    ASSERT_TRUE(addressToData(TWCoinTypeCosmos, "fake").empty());
}

TEST(CosmosAddress, Valid) {
    ASSERT_TRUE(Address::isValid(TWCoinTypeBinance, "bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2"));
}

TEST(CosmosAddress, Invalid) {
    ASSERT_FALSE(Address::isValid(TWCoinTypeBinance, "bnb1grpf0955h0ykzq3ar6nmum7y6gdfl6lxfn46h2"));
}

TEST(CosmosAddress, Cosmos_FromPublicKey) {
    auto privateKey = PrivateKey(parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005"));
    auto publicKeyData = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    ASSERT_EQ(hex(publicKeyData.bytes), "0257286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc5");

    auto publicKey = PublicKey(publicKeyData);
    auto address = Address("cosmos", publicKey);
    ASSERT_EQ(address.string(), "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02");
    ASSERT_EQ(hex(address.getKeyHash()), "bc2da90c84049370d1b7c528bc164bc588833f21");
}

TEST(CosmosAddress, Cosmos_FromString) {
    Address addr;
    ASSERT_TRUE(Address::decode("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02", addr));
    ASSERT_EQ(addr.string(), "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02");
}

TEST(CosmosAddress, Cosmos_Valid) {
    ASSERT_TRUE(Address::isValid(TWCoinTypeCosmos, "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02"));
    ASSERT_TRUE(Address::isValid(TWCoinTypeCosmos, "cosmospub1addwnpepqftjsmkr7d7nx4tmhw4qqze8w39vjq364xt8etn45xqarlu3l2wu2n7pgrq"));
    ASSERT_TRUE(Address::isValid(TWCoinTypeCosmos, "cosmosvaloper1sxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08"));
    ASSERT_TRUE(Address::isValid(TWCoinTypeCosmos, "cosmosvalconspub1zcjduepqjnnwe2jsywv0kfc97pz04zkm7tc9k2437cde2my3y5js9t7cw9mstfg3sa"));
}

TEST(CosmosAddress, Cosmos_Invalid) {
    ASSERT_FALSE(Address::isValid(TWCoinTypeCosmos, "cosmos1xsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02"));
    ASSERT_FALSE(Address::isValid(TWCoinTypeCosmos, "cosmospub1xddwnpepqftjsmkr7d7nx4tmhw4qqze8w39vjq364xt8etn45xqarlu3l2wu2n7pgrq"));
    ASSERT_FALSE(Address::isValid(TWCoinTypeCosmos, "cosmosvaloper1xxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08"));
    ASSERT_FALSE(Address::isValid(TWCoinTypeCosmos, "cosmosvalconspub1xcjduepqjnnwe2jsywv0kfc97pz04zkm7tc9k2437cde2my3y5js9t7cw9mstfg3sa"));
}

TEST(CosmosAddress, ThorFromPublicKey) {
    auto privateKey = PrivateKey(parse_hex("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e"));
    auto publicKeyData = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    ASSERT_EQ(hex(publicKeyData.bytes), "03ed997e396cf4292f5fce5a42bba41599ccd5d96e313154a7c9ea7049de317c77");

    auto publicKey = PublicKey(publicKeyData);
    auto address = Address("thor", publicKey);
    ASSERT_EQ(address.string(), "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r");
    ASSERT_EQ(hex(address.getKeyHash()), "1522e767db6eb19708b0038029bfbd607bc9bd0e");
}

TEST(CosmosAddress, ThorValid) {
    ASSERT_TRUE(Address::isValid(TWCoinTypeTHORChain, "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r"));
    ASSERT_FALSE(Address::isValid(TWCoinTypeTHORChain, "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2s"));
}

} // namespace TW::Cosmos
