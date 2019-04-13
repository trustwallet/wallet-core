// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

#include "NEO/Address.h"

using namespace TW::NEO;

namespace TW {

TEST(Coin, ValidateAddressEthereum) {
    EXPECT_TRUE(validateAddress(TWCoinTypeEthereum, "0xeDe8F58dADa22c3A49dB60D4f82BAD428ab65F89"));
    EXPECT_FALSE(validateAddress(TWCoinTypeEthereum, "ede8f58dada22a49db60d4f82bad428ab65f89"));
}

TEST(Coin, validateAddressBitcoin) {
    EXPECT_TRUE(validateAddress(TWCoinTypeBitcoin, "bc1q2ddhp55sq2l4xnqhpdv0xazg02v9dr7uu8c2p2"));
    EXPECT_TRUE(validateAddress(TWCoinTypeBitcoin, "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2"));
    EXPECT_TRUE(validateAddress(TWCoinTypeBitcoin, "3J98t1WpEZ73CNmQviecrnyiWrnqRhWNLy"));

    EXPECT_FALSE(validateAddress(TWCoinTypeBitcoin, "bc1q2ddhp55sq2l4xnqhpdv9xazg02v9dr7uu8c2p2"));
    EXPECT_FALSE(validateAddress(TWCoinTypeBitcoin, "MPmoY6RX3Y3HFjGEnFxyuLPCQdjvHwMEny"));
}

TEST(Coin, ValidateAddressLitecoin) {
    EXPECT_TRUE(validateAddress(TWCoinTypeLitecoin, "ltc1q5wmm9vrz55war9c0rgw26tv9un5fxnn7slyjpy"));
    EXPECT_TRUE(validateAddress(TWCoinTypeLitecoin, "MPmoY6RX3Y3HFjGEnFxyuLPCQdjvHwMEny"));

    EXPECT_FALSE(validateAddress(TWCoinTypeLitecoin, "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2"));
}

TEST(Coin, ValidateAddressViacoin) {
    EXPECT_TRUE(validateAddress(TWCoinTypeViacoin, "via1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z09y9mn2"));
    EXPECT_TRUE(validateAddress(TWCoinTypeViacoin, "Vw6bJFaF5Hyiveko7dpqRjVvcTAsjz7eYa"));

    EXPECT_FALSE(validateAddress(TWCoinTypeViacoin, "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2"));
}

TEST(Coin, ValidateAddressBitcoinCash) {
    EXPECT_TRUE(validateAddress(TWCoinTypeBitcoinCash, "bitcoincash:qruxj7zq6yzpdx8dld0e9hfvt7u47zrw9gfr5hy0vh"));
    EXPECT_TRUE(validateAddress(TWCoinTypeBitcoinCash, "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2"));
    EXPECT_TRUE(validateAddress(TWCoinTypeBitcoinCash, "qq07l6rr5lsdm3m80qxw80ku2ex0tj76vvsxpvmgme"));
    EXPECT_TRUE(validateAddress(TWCoinTypeBitcoinCash, "qqslmu0jxk4st3ldjyuazfpf5thd6vlgfuggqd3re4"));
}

TEST(Coin, ValidateAddressDogecoin) {
    EXPECT_TRUE(validateAddress(TWCoinTypeDogecoin, "DHKM6NDUUv9kaHAGi1QU7MRBNKfQiAdP3F"));

    EXPECT_FALSE(validateAddress(TWCoinTypeDogecoin, "NmH7tmeJUmHcncBDvpr7aJNEBk7rp5zYsB1qt"));
    EXPECT_FALSE(validateAddress(TWCoinTypeDogecoin, "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3AAAA"));
    EXPECT_FALSE(validateAddress(TWCoinTypeDogecoin, "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2"));
}

TEST(Coin, ValidateAddressTezos) {
    EXPECT_TRUE(validateAddress(TWCoinTypeTezos, "tz1d1qQL3mYVuiH4JPFvuikEpFwaDm85oabM"));

    EXPECT_FALSE(validateAddress(TWCoinTypeTezos, "NmH7tmeJUmHcncBDvpr7aJNEBk7rp5zYsB1qt"));
    EXPECT_FALSE(validateAddress(TWCoinTypeTezos, "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3AAAA"));
    EXPECT_FALSE(validateAddress(TWCoinTypeTezos, "1tzeZwq8b5cvE2bPKokatLkVMzkxz24zAAAAA"));
}

TEST(Coin, validateAddressZcash) {
    EXPECT_TRUE(validateAddress(TWCoinTypeZcash, "t3WeKQDxCijL5X7rwFem1MTL9ZwVJkUFhpF"));
    EXPECT_TRUE(validateAddress(TWCoinTypeZcash, "t1aQ1JEFMqciA58XU6CR8CNohAYzESm8c1L"));
    EXPECT_FALSE(validateAddress(TWCoinTypeZcash, "1Ma2DrB78K7jmAwaomqZNRMCvgQrNjE2QC"));
}

TEST(Coin, validateAddressOntology){
    EXPECT_TRUE(validateAddress(TWCoinTypeOntology, "AW2f6d82p7dC3jFMrzR5x5GhyTWSB5pxVz"));
    EXPECT_FALSE(validateAddress(TWCoinTypeOntology,"BW2f6d82p7dC3jFMrzR5x5GhyTWSB5pxVz"));
    EXPECT_FALSE(validateAddress(TWCoinTypeOntology,"4646464646464646464646464646464646464646464646464646464646464646"));
}

TEST(Coin, validateAddressIOST) {
    EXPECT_TRUE(validateAddress(TWCoinTypeIOST, "lispczz"));
    EXPECT_FALSE(validateAddress(TWCoinTypeIOST,"nil"));
    EXPECT_FALSE(validateAddress(TWCoinTypeIOST,"thisisaverylongstring"));
    EXPECT_FALSE(validateAddress(TWCoinTypeIOST,"invalid character"));
    EXPECT_FALSE(validateAddress(TWCoinTypeIOST,"BIGCASE"));
}

TEST(Coin, validateAddressGroestlcoin){
    EXPECT_TRUE(validateAddress(TWCoinTypeGroestlcoin, "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM"));
    EXPECT_FALSE(validateAddress(TWCoinTypeGroestlcoin,"Fj62rBJi8LvbmWu2jzkaUX1NFXLEsNpjgw")); // sha256d checksum instead of groestl512d
    EXPECT_FALSE(validateAddress(TWCoinTypeGroestlcoin,"mvbu1Gdy8SUjTenqerxUaZyYjmvedc787y")); // valid checksum, but testnet prefix
    EXPECT_TRUE(validateAddress(TWCoinTypeGroestlcoin, "31inaRqambLsd9D7Ke4USZmGEVd3PHkh7P"));
    EXPECT_FALSE(validateAddress(TWCoinTypeGroestlcoin,"31inaRqambLsd9D7Ke4USZmGEVd3LVt8yd")); // sha256d checksum instead of groestl512d
    EXPECT_FALSE(validateAddress(TWCoinTypeGroestlcoin,"2N4Q5FhU2497BryFfUgbqkAJE87aKDv3V3e")); // valid checksum, but testnet prefix
    EXPECT_TRUE(validateAddress(TWCoinTypeGroestlcoin, "grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne"));
    EXPECT_FALSE(validateAddress(TWCoinTypeGroestlcoin,"bc1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z00ppggv"));
}

TEST(Coin, validateAddressQtum) {
    EXPECT_TRUE(validateAddress(TWCoinTypeQtum, "qc1qn9gjawre2t6xmcv5gyqkujqhd8cfvvyx0rx2mp"));
    EXPECT_TRUE(validateAddress(TWCoinTypeQtum, "Qbmj3ufB1TaRSSP5DYR4KQxsyHBNrk8Y4p"));

    EXPECT_FALSE(validateAddress(TWCoinTypeQtum, "Qb4j3ufB1TaRSSP5DYR4KQxsyHBNrk8Y4p"));
    EXPECT_FALSE(validateAddress(TWCoinTypeQtum, "qc2qn9gjawre2t6xmcv5gyqkujqhd8cfvvyx0rx2mp"));
}

TEST(Coin, DeriveAddress) {
    const auto privateKey = PrivateKey(parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646"));
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeAion, privateKey), "0xa0010b0ea04ba4d76ca6e5e9900bacf19bc4402eaec7e36ea7ddd8eed48f60f3");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeBinance, privateKey), "tbnb1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z042ftd7");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeBitcoin, privateKey), "bc1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z00ppggv");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeBitcoinCash, privateKey), "bitcoincash:qz7eyzytkl5z6cg6nw20hd62pyyp22mcfuardfd2vn");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeCallisto, privateKey), "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeDash, privateKey), "XsyCV5yojxF4y3bYeEiVYqarvRgsWFELZL");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeDecred, privateKey), "Dsp4u8xxTHSZU2ELWTQLQP77xJhgeWrTsGK");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeEthereum, privateKey), "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeEthereumClassic, privateKey), "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeGo, privateKey), "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeGroestlcoin, privateKey), "grs1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z0jsaf3d");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeICON, privateKey), "hx4728fc65c31728f0d3538b8783b5394b31a136b9");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeIOST, privateKey), "aHcyrBUD7o2PY319zAA9Y2DFMpG3LqDyc42ubyugyoPd");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeLitecoin, privateKey), "ltc1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z0tamvsu");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeViacoin, privateKey), "via1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z09y9mn2");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeNimiq, privateKey), "NQ74 D40G N3M0 9EJD ET56 UPLR 02VC X6DU 8G1E");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeOntology, privateKey), "AeicEjZyiXKgUeSBbYQHxsU1X3V5Buori5");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypePoa, privateKey), "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeRipple, privateKey), "rJHMeqKu8Ep7Fazx8MQG6JunaafBXz93YQ");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeStellar, privateKey), "GDXJHJHWN6GRNOAZXON6XH74ZX6NYFAS5B7642RSJQVJTIPA4ZYUQLEB");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeTezos, privateKey), "tz1gcEWswVU6dxfNQWbhTgaZrUrNUFwrsT4z");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeThunderToken, privateKey), "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeTomoChain, privateKey), "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeTron, privateKey), "TQLCsShbQNXMTVCjprY64qZmEA4rBarpQp");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeVeChain, privateKey), "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeWanChain, privateKey), "0x9D8a62F656A8D1615c1294FD71E9cfB3e4855a4f");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeXDai, privateKey), "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeZcash, privateKey), "t1b9xfAk3kZp5Qk3rinDPq7zzLkJGHTChDS");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeZcoin, privateKey), "aHzpPjmY132KseS4nkiQTbDahTEXqesY89");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeNEO, privateKey), "AeicEjZyiXKgUeSBbYQHxsU1X3V5Buori5");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeKIN, privateKey), "GDXJHJHWN6GRNOAZXON6XH74ZX6NYFAS5B7642RSJQVJTIPA4ZYUQLEB");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeTheta, privateKey), "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F");
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeQtum, privateKey), "QdtLm8ccxhuJFF5zCgikpaghbM3thdaGsW");
}

} // namespace TW
