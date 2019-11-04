// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bech32Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

using namespace TW;

TEST(Bech32Address, Valid) {
    ASSERT_TRUE(Bech32Address::isValid("bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2", "bnb"));
    
    ASSERT_TRUE(Bech32Address::isValid("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02", "cosmos"));
    ASSERT_TRUE(Bech32Address::isValid("cosmospub1addwnpepqftjsmkr7d7nx4tmhw4qqze8w39vjq364xt8etn45xqarlu3l2wu2n7pgrq", "cosmos"));
    ASSERT_TRUE(Bech32Address::isValid("cosmosvaloper1sxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08", "cosmos"));
    ASSERT_TRUE(Bech32Address::isValid("cosmosvalconspub1zcjduepqjnnwe2jsywv0kfc97pz04zkm7tc9k2437cde2my3y5js9t7cw9mstfg3sa", "cosmos"));

    ASSERT_TRUE(Bech32Address::isValid("one1a50tun737ulcvwy0yvve0pvu5skq0kjargvhwe", "one"));
    ASSERT_TRUE(Bech32Address::isValid("one1tp7xdd9ewwnmyvws96au0e7e7mz6f8hjqr3g3p", "one"));

    ASSERT_TRUE(Bech32Address::isValid("zil1fwh4ltdguhde9s7nysnp33d5wye6uqpugufkz7", "zil"));
}

TEST(Bech32Address, Invalid) {
    ASSERT_FALSE(Bech32Address::isValid("bnb1grpf0955h0ykzq3ar6nmum7y6gdfl6lxfn46h2", "bnb"));

    ASSERT_FALSE(Bech32Address::isValid("cosmos1xsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02", "cosmos"));
    ASSERT_FALSE(Bech32Address::isValid("cosmospub1xddwnpepqftjsmkr7d7nx4tmhw4qqze8w39vjq364xt8etn45xqarlu3l2wu2n7pgrq", "cosmos"));
    ASSERT_FALSE(Bech32Address::isValid("cosmosvaloper1xxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08", "cosmos"));
    ASSERT_FALSE(Bech32Address::isValid("cosmosvalconspub1xcjduepqjnnwe2jsywv0kfc97pz04zkm7tc9k2437cde2my3y5js9t7cw9mstfg3sa", "cosmos"));

    ASSERT_FALSE(Bech32Address::isValid("one1a50tun737ulcvwy0yvve0pe", "one"));
    ASSERT_FALSE(Bech32Address::isValid("oe1tp7xdd9ewwnmyvws96au0ee7e7mz6f8hjqr3g3p", "one"));

    ASSERT_FALSE(Bech32Address::isValid("0x91cddcebe846ce4d47712287eee53cf17c2cfb7"));
    ASSERT_FALSE(Bech32Address::isValid(""));
    ASSERT_FALSE(Bech32Address::isValid("0x"));
    ASSERT_FALSE(Bech32Address::isValid("91cddcebe846ce4d47712287eee53cf17c2cfb7"));
}

void TestDecodeFromString(const char* stringAddr, const char* hrp, const char* expectdKey) {
    Bech32Address address("");
    ASSERT_TRUE(Bech32Address::decode(stringAddr, address, hrp));
    ASSERT_EQ(expectdKey, hex(address.getKeyHash()));
}

TEST(Bech32Address, FromString) {
    TestDecodeFromString("one1a50tun737ulcvwy0yvve0pvu5skq0kjargvhwe", "one", "ed1ebe4fd1f73f86388f231997859ca42c07da5d");
}

TEST(Bech32Address, FromKeyHash) {
    {
        auto address = Bech32Address("bnb", parse_hex("b6561dcc104130059a7c08f48c64610c1f6f9064"));
        ASSERT_EQ("bnb1ketpmnqsgycqtxnupr6gcerpps0klyryuudz05", address.string());
    }
    {
        const auto address = Bech32Address("one", parse_hex("0x587c66b4b973a7b231d02ebbc7e7d9f6c5a49ef2"));
        ASSERT_EQ("one1tp7xdd9ewwnmyvws96au0e7e7mz6f8hjqr3g3p", address.string());
    }
    {
        const auto address = Bech32Address("zil", parse_hex("0x91cdDcEBE846ce4d47712287EEe53cF17c2cfB77"));
        ASSERT_EQ("zil1j8xae6lggm8y63m3y2r7aefu797ze7mhzulnqg", address.string());
        const auto address2 = Bech32Address("zil", parse_hex("1d19918a737306218b5cbb3241fcdcbd998c3a72"));
        ASSERT_EQ("zil1r5verznnwvrzrz6uhveyrlxuhkvccwnju4aehf", address2.string());
    }
}

TEST(Bech32Address, FromPublicKey) {
    {
        auto privateKey = PrivateKey(parse_hex("95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832"));
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        ASSERT_EQ(hex(publicKey.bytes.begin(), publicKey.bytes.end()), "026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e502");
        auto address = Bech32Address("bnb", HASHER_SHA2_RIPEMD, publicKey);
        ASSERT_EQ("bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2", address.string());
    }
    {
        auto privateKey = PrivateKey(parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005"));
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        ASSERT_EQ(hex(publicKey.bytes.begin(), publicKey.bytes.end()), "0257286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc5");
        auto address = Bech32Address("cosmos", HASHER_SHA2_RIPEMD, publicKey);
        ASSERT_EQ(address.string(), "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02");
    }
    {
        auto privateKey = PrivateKey(parse_hex("e2f88b4974ae763ca1c2db49218802c2e441293a09eaa9ab681779e05d1b7b94"));
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
        auto address = Bech32Address("one", HASHER_SHA3K, publicKey);
        ASSERT_EQ(address.string(), "one1a50tun737ulcvwy0yvve0pvu5skq0kjargvhwe");
    }
    {
        const auto privateKey = PrivateKey(parse_hex("3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6"));
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        ASSERT_EQ(hex(publicKey.bytes.begin(), publicKey.bytes.end()), "02b65744e8bd0ba7666468abaff2aeb862c88a25ed605e0153100aa8f2661c1c3d");
        const auto address = Bech32Address("zil", HASHER_SHA2, publicKey);
        ASSERT_EQ("zil", address.getHrp());
        ASSERT_EQ("zil1j8xae6lggm8y63m3y2r7aefu797ze7mhzulnqg", address.string());
    }
}
