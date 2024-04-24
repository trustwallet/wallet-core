// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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

    ASSERT_TRUE(Bech32Address::isValid("io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j", "io"));

    ASSERT_TRUE(Bech32Address::isValid("zil1fwh4ltdguhde9s7nysnp33d5wye6uqpugufkz7", "zil"));

    ASSERT_TRUE(Bech32Address::isValid("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz", "erd"));
    ASSERT_TRUE(Bech32Address::isValid("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r", "erd"));
    ASSERT_TRUE(Bech32Address::isValid("erd19nu5t7hszckwah5nlcadmk5rlchtugzplznskffpwecygcu0520s9tnyy0", "erd"));

    ASSERT_TRUE(Bech32Address::isValid("BNB1GRPF0955H0YKZQ3AR5NMUM7Y6GDFL6LXFN46H2", "bnb")); // uppercase version
}

TEST(Bech32Address, Invalid) {
    ASSERT_FALSE(Bech32Address::isValid("bnb1grpf0955h0ykzq3ar6nmum7y6gdfl6lxfn46h2", "bnb")); // 1-char diff
    ASSERT_FALSE(Bech32Address::isValid("bnb1grPF0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2", "bnb")); // mixed case

    ASSERT_FALSE(Bech32Address::isValid("cosmos1xsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02", "cosmos"));
    ASSERT_FALSE(Bech32Address::isValid("cosmospub1xddwnpepqftjsmkr7d7nx4tmhw4qqze8w39vjq364xt8etn45xqarlu3l2wu2n7pgrq", "cosmos"));
    ASSERT_FALSE(Bech32Address::isValid("cosmosvaloper1xxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08", "cosmos"));
    ASSERT_FALSE(Bech32Address::isValid("cosmosvalconspub1xcjduepqjnnwe2jsywv0kfc97pz04zkm7tc9k2437cde2my3y5js9t7cw9mstfg3sa", "cosmos"));

    ASSERT_FALSE(Bech32Address::isValid("one1a50tun737ulcvwy0yvve0pe", "one"));
    ASSERT_FALSE(Bech32Address::isValid("oe1tp7xdd9ewwnmyvws96au0ee7e7mz6f8hjqr3g3p", "one"));

    ASSERT_FALSE(Bech32Address::isValid("io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8", "io"));
    ASSERT_FALSE(Bech32Address::isValid("io187wzp08vnhjpkydnr97qlh8kh0dpkkytfam8j", "io"));
    ASSERT_FALSE(Bech32Address::isValid("it187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j", "io"));

    ASSERT_FALSE(Bech32Address::isValid("0x91cddcebe846ce4d47712287eee53cf17c2cfb7"));
    ASSERT_FALSE(Bech32Address::isValid(""));
    ASSERT_FALSE(Bech32Address::isValid("0x"));
    ASSERT_FALSE(Bech32Address::isValid("91cddcebe846ce4d47712287eee53cf17c2cfb7"));

    ASSERT_FALSE(Bech32Address::isValid("", "erd"));
    ASSERT_FALSE(Bech32Address::isValid("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35!", "erd"));
    ASSERT_FALSE(Bech32Address::isValid("xerd19nu5t7hszckwah5nlcadmk5rlchtugzplznskffpwecygcu0520s9tnyy0", "erd"));
}

TEST(Bech32Address, InvalidWrongPrefix) {
    ASSERT_TRUE(Bech32Address::isValid("one1a50tun737ulcvwy0yvve0pvu5skq0kjargvhwe", "one"));
    ASSERT_FALSE(Bech32Address::isValid("one1a50tun737ulcvwy0yvve0pvu5skq0kjargvhwe", "two"));
}

void TestDecodeFromString(const char* stringAddr, const char* hrp, const char* expectdKeyHash) {
    Bech32Address address("");
    // decode
    ASSERT_TRUE(Bech32Address::decode(stringAddr, address, hrp));
    // verify expected hash
    ASSERT_EQ(expectdKeyHash, hex(address.getKeyHash()));
    // verify back to string
    ASSERT_EQ(stringAddr, address.string());
}

TEST(Bech32Address, FromString) {
    TestDecodeFromString("one1a50tun737ulcvwy0yvve0pvu5skq0kjargvhwe", "one", "ed1ebe4fd1f73f86388f231997859ca42c07da5d");

    TestDecodeFromString("io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j", "io", "3f9c20bcec9de520d88d98cbe07ee7b5ded0dac4");
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
        ASSERT_EQ(hex(publicKey.bytes), "026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e502");
        auto address = Bech32Address("bnb", Hash::HasherSha256ripemd, publicKey);
        ASSERT_EQ("bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2", address.string());
    }
    {
        auto privateKey = PrivateKey(parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005"));
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        ASSERT_EQ(hex(publicKey.bytes), "0257286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc5");
        auto address = Bech32Address("cosmos", Hash::HasherSha256ripemd, publicKey);
        ASSERT_EQ(address.string(), "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02");
    }
    {
        auto privateKey = PrivateKey(parse_hex("e2f88b4974ae763ca1c2db49218802c2e441293a09eaa9ab681779e05d1b7b94"));
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
        auto address = Bech32Address("one", Hash::HasherKeccak256, publicKey);
        ASSERT_EQ(address.string(), "one1a50tun737ulcvwy0yvve0pvu5skq0kjargvhwe");
    }
    {
        auto privateKey = PrivateKey(parse_hex("0806c458b262edd333a191e92f561aff338211ee3e18ab315a074a2d82aa343f"));
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
        auto address = Bech32Address("io", Hash::HasherKeccak256, publicKey);
        ASSERT_EQ(address.string(), "io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j");
    }
    {
        const auto privateKey = PrivateKey(parse_hex("3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6"));
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        ASSERT_EQ(hex(publicKey.bytes), "02b65744e8bd0ba7666468abaff2aeb862c88a25ed605e0153100aa8f2661c1c3d");
        const auto address = Bech32Address("zil", Hash::HasherSha256, publicKey);
        ASSERT_EQ("zil", address.getHrp());
        ASSERT_EQ("zil1j8xae6lggm8y63m3y2r7aefu797ze7mhzulnqg", address.string());
    }
}

// From same public key, but different hashes: different results
TEST(Bech32Address, Hashes) {
    const auto privateKey = PrivateKey(parse_hex("3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6"));

    auto publicKey1 = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    ASSERT_EQ("02b65744e8bd0ba7666468abaff2aeb862c88a25ed605e0153100aa8f2661c1c3d", hex(publicKey1.bytes));

    const auto address1 = Bech32Address("hrp", Hash::HasherSha256ripemd, publicKey1);
    ASSERT_EQ("hrp186zwn9h0z9fyvwfqs4jl92cw3kexusm4xw6ptp", address1.string());

    const auto address2 = Bech32Address("hrp", Hash::HasherSha256, publicKey1);
    ASSERT_EQ("hrp1j8xae6lggm8y63m3y2r7aefu797ze7mhgfetvu", address2.string());

    auto publicKey2 = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
    ASSERT_EQ(
        "04b65744e8bd0ba7666468abaff2aeb862c88a25ed605e0153100aa8f2661c1c3d83c307736082c09f1f22328e0fbeab40ddd198cf0f70fcdaa1e5969ca400c098",
        hex(publicKey2.bytes));

    const auto address3 = Bech32Address("hrp", Hash::HasherKeccak256, publicKey2);
    ASSERT_EQ("hrp17hff3s97m5uxpjcdq3nzqxxatt8cmumnsf03su", address3.string());
}

// From same public key, but different prefixes: different results (checksum)
TEST(Bech32Address, Prefixes) {
    const auto privateKey = PrivateKey(parse_hex("3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6"));

    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    ASSERT_EQ("02b65744e8bd0ba7666468abaff2aeb862c88a25ed605e0153100aa8f2661c1c3d", hex(publicKey.bytes));

    const auto address1 = Bech32Address("hrpone", Hash::HasherSha256ripemd, publicKey);
    ASSERT_EQ("hrpone186zwn9h0z9fyvwfqs4jl92cw3kexusm47das6p", address1.string());
    const auto address2 = Bech32Address("hrptwo", Hash::HasherSha256ripemd, publicKey);
    ASSERT_EQ("hrptwo186zwn9h0z9fyvwfqs4jl92cw3kexusm4qzr8p7", address2.string());
    const auto address3 = Bech32Address("hrpthree", Hash::HasherSha256ripemd, publicKey);
    ASSERT_EQ("hrpthree186zwn9h0z9fyvwfqs4jl92cw3kexusm4wuqkvd", address3.string());
}
