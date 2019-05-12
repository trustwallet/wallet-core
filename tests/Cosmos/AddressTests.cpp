// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Cosmos/Address.h"

#include <gtest/gtest.h>
#include <TrustWalletCore/TWHDWallet.h>

namespace TW::Cosmos {

TEST(Address, Valid) {
    ASSERT_TRUE(Address::isValid("bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2"));
}

TEST(Address, Invalid) {
    ASSERT_FALSE(Address::isValid("bnb1grpf0955h0ykzq3ar6nmum7y6gdfl6lxfn46h2"));
}

TEST(Address, FromKeyHash) {
    auto privateKey = PrivateKey(parse_hex("95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832"));
    auto publicKeyData = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    ASSERT_EQ(hex(publicKeyData.bytes.begin(), publicKeyData.bytes.end()), "026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e502");

    auto publicKey = PublicKey(publicKeyData);
    auto address = Address("bnb", publicKey);
    ASSERT_EQ(address.string(), "bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2");
}

TEST(Address, Cosmos_FromKeyHash) {
    auto privateKey = PrivateKey(parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005"));
    auto publicKeyData = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    ASSERT_EQ(hex(publicKeyData.bytes.begin(), publicKeyData.bytes.end()), "0257286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc5");

    auto publicKey = PublicKey(publicKeyData);
    auto address = Address("cosmos", publicKey);
    ASSERT_EQ(address.string(), "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02");
}

TEST(Address, Cosmos_Valid) {
    ASSERT_TRUE(Address::isValid("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02"));
    ASSERT_TRUE(Address::isValid("cosmospub1addwnpepqftjsmkr7d7nx4tmhw4qqze8w39vjq364xt8etn45xqarlu3l2wu2n7pgrq"));
    ASSERT_TRUE(Address::isValid("cosmosvaloper1sxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08"));
    ASSERT_TRUE(Address::isValid("cosmosvalconspub1zcjduepqjnnwe2jsywv0kfc97pz04zkm7tc9k2437cde2my3y5js9t7cw9mstfg3sa"));
}

TEST(Address, Cosmos_Invalid) {
    ASSERT_FALSE(Address::isValid("cosmos1xsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02"));
    ASSERT_FALSE(Address::isValid("cosmospub1xddwnpepqftjsmkr7d7nx4tmhw4qqze8w39vjq364xt8etn45xqarlu3l2wu2n7pgrq"));
    ASSERT_FALSE(Address::isValid("cosmosvaloper1xxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08"));
    ASSERT_FALSE(Address::isValid("cosmosvalconspub1xcjduepqjnnwe2jsywv0kfc97pz04zkm7tc9k2437cde2my3y5js9t7cw9mstfg3sa"));
}
} // namespace TW::Cosmos
