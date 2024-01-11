// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW {

TEST(Coin, ValidateAddressAion) {
    EXPECT_TRUE(validateAddress(TWCoinTypeAion, "0xa0d5b10ee59fb918f7c7d0ba96387e4b1539fa8c7466625c39efdac2286cdfa1"));
    EXPECT_FALSE(validateAddress(TWCoinTypeAion, "0xeDe8F58dADa22c3A49dB60D4f82BAD428ab65F89"));
}

TEST(Coin, ValidateAddressZilliqa) {
    EXPECT_TRUE(validateAddress(TWCoinTypeZilliqa, "zil1j8xae6lggm8y63m3y2r7aefu797ze7mhzulnqg"));
    EXPECT_FALSE(validateAddress(TWCoinTypeZilliqa, "0x91cddcebe846ce4d47712287eee53cf17c2cfb77"));
    EXPECT_FALSE(validateAddress(TWCoinTypeZilliqa, "91cddcebe846ce4d47712287eee53cf17c2cfb77"));
    EXPECT_FALSE(validateAddress(TWCoinTypeZilliqa, "0x"));
    EXPECT_FALSE(validateAddress(TWCoinTypeZilliqa, ""));
}

TEST(Coin, ValidateAddressEthereum) {
    EXPECT_TRUE(validateAddress(TWCoinTypeEthereum, "0xeDe8F58dADa22c3A49dB60D4f82BAD428ab65F89"));
    EXPECT_FALSE(validateAddress(TWCoinTypeEthereum, "ede8f58dada22a49db60d4f82bad428ab65f89"));

    ASSERT_EQ(normalizeAddress(TWCoinTypeEthereum, "0x7d8bf18c7ce84b3e175b339c4ca93aed1dd166f1"), "0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1");
}

TEST(Coin, validateAddressBitcoin) {
    EXPECT_TRUE(validateAddress(TWCoinTypeBitcoin, "bc1q2ddhp55sq2l4xnqhpdv0xazg02v9dr7uu8c2p2"));
    EXPECT_TRUE(validateAddress(TWCoinTypeBitcoin, "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2"));
    EXPECT_TRUE(validateAddress(TWCoinTypeBitcoin, "3J98t1WpEZ73CNmQviecrnyiWrnqRhWNLy"));

    EXPECT_FALSE(validateAddress(TWCoinTypeBitcoin, "bc1q2ddhp55sq2l4xnqhpdv9xazg02v9dr7uu8c2p2"));
    EXPECT_FALSE(validateAddress(TWCoinTypeBitcoin, "MPmoY6RX3Y3HFjGEnFxyuLPCQdjvHwMEny"));

    ASSERT_EQ(normalizeAddress(TWCoinTypeBitcoin, "3J98t1WpEZ73CNmQviecrnyiWrnqRhWNLy"), "3J98t1WpEZ73CNmQviecrnyiWrnqRhWNLy");
    ASSERT_EQ(normalizeAddress(TWCoinTypeBitcoin, "MPmoY6RX3Y3HFjGEnFxyuLPCQdjvHwMEny"), "");
}

TEST(Coin, ValidateAddressBinance) {
    EXPECT_TRUE(validateAddress(TWCoinTypeBinance, "bnb12vtaxl9952zm6rwf7v8jerq74pvaf77fcmvzhw"));
    EXPECT_TRUE(validateAddress(TWCoinTypeBinance, "tbnb1devga6q804tx9fqrnx0vtu5r36kxgp9t4ruzk2", "tbnb"));

    EXPECT_FALSE(validateAddress(TWCoinTypeBinance, "tbnb1devga6q804tx9fqrnx0vtu5r36kxgp9t4ruzk2"));
    EXPECT_FALSE(validateAddress(TWCoinTypeBinance, "bad1devga6q804tx9fqrnx0vtu5r36kxgp9tqx8h9k"));
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

    ASSERT_EQ(normalizeAddress(TWCoinTypeBitcoinCash, "qqslmu0jxk4st3ldjyuazfpf5thd6vlgfuggqd3re4"), "bitcoincash:qqslmu0jxk4st3ldjyuazfpf5thd6vlgfuggqd3re4");
    ASSERT_EQ(normalizeAddress(TWCoinTypeBitcoinCash, "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2"), "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2");
}

TEST(Coin, ValidateAddressDogecoin) {
    EXPECT_TRUE(validateAddress(TWCoinTypeDogecoin, "DHKM6NDUUv9kaHAGi1QU7MRBNKfQiAdP3F"));

    EXPECT_FALSE(validateAddress(TWCoinTypeDogecoin, "NmH7tmeJUmHcncBDvpr7aJNEBk7rp5zYsB1qt"));
    EXPECT_FALSE(validateAddress(TWCoinTypeDogecoin, "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3AAAA"));
    EXPECT_FALSE(validateAddress(TWCoinTypeDogecoin, "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2"));
}

TEST(Coin, ValidateAddressDecred) {
    EXPECT_TRUE(validateAddress(TWCoinTypeDecred, "DsZgSbMPgKKJVm2j9Vuk3VEUBykkw72a56D"));
    EXPECT_TRUE(validateAddress(TWCoinTypeDecred, "Dcur2mcGjmENx4DhNqDctW5wJCVyT3Qeqkx"));

    EXPECT_FALSE(validateAddress(TWCoinTypeDecred, "tz1d1qQL3mYVuiH4JPFvuikEpFwaDm85oabM"));
    EXPECT_FALSE(validateAddress(TWCoinTypeDecred, "DHKM6NDUUv9kaHAGi1QU7MRBNKfQiAdP3F"));
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

TEST(Coin, ValidateAddressIoTeX) {
    EXPECT_TRUE(validateAddress(TWCoinTypeIoTeX, "io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j"));
    EXPECT_FALSE(validateAddress(TWCoinTypeIoTeX, "io187wzp08vnhjbpkydnr97qlh8kh0dpkkytfam8j"));
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

TEST(Coin, validateAddressNULS){
    EXPECT_TRUE(validateAddress(TWCoinTypeNULS, "NULSd6HgbwcM8wz48f6UkFYHLVriT1L81X9z2"));
    EXPECT_FALSE(validateAddress(TWCoinTypeNULS, "NULSd6HgUxmcJWc88iELEJ7RH9XHsazBQqnJd"));
}

TEST(Coin, validateAddressQtum) {
    EXPECT_TRUE(validateAddress(TWCoinTypeQtum, "qc1qn9gjawre2t6xmcv5gyqkujqhd8cfvvyx0rx2mp"));
    EXPECT_TRUE(validateAddress(TWCoinTypeQtum, "Qbmj3ufB1TaRSSP5DYR4KQxsyHBNrk8Y4p"));

    EXPECT_FALSE(validateAddress(TWCoinTypeQtum, "Qb4j3ufB1TaRSSP5DYR4KQxsyHBNrk8Y4p"));
    EXPECT_FALSE(validateAddress(TWCoinTypeQtum, "qc2qn9gjawre2t6xmcv5gyqkujqhd8cfvvyx0rx2mp"));
}

TEST(Coin, ValidateAddressEOS) {
    EXPECT_TRUE(validateAddress(TWCoinTypeEOS, "EOS65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjTF"));
    EXPECT_TRUE(validateAddress(TWCoinTypeEOS, "PUB_R1_5hieQEFWh68h6bjaYAY25Ptd2bmqLCaFsunaneh9gZsmSgUBUe"));

    EXPECT_FALSE(validateAddress(TWCoinTypeEOS, "PUB_K1_5hieQEFWh68h6bjaYAY25Ptd2bmqLCaFsunaneh9gZsmSgUBUe"));
    EXPECT_FALSE(validateAddress(TWCoinTypeEOS, "EOS65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjT"));
    EXPECT_FALSE(validateAddress(TWCoinTypeEOS, "FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o"));
    EXPECT_FALSE(validateAddress(TWCoinTypeEOS, "STM5LZBF18uucMKVyKMQ7r6qnXBWn9StM5AJYHs1kidPeyNexuBUm"));
}

TEST(Coin, ValidateAddressNano) {
    EXPECT_TRUE(validateAddress(TWCoinTypeNano, "nano_1nanode8ngaakzbck8smq6ru9bethqwyehomf79sae1k7xd47dkidjqzffeg"));
    EXPECT_TRUE(validateAddress(TWCoinTypeNano, "xrb_1111111111111111111111111111111111111111111111111111hifc8npp"));

    EXPECT_FALSE(validateAddress(TWCoinTypeNano, "nano_1111111111111111111111111111111111111111111111111111hi111111"));
    EXPECT_FALSE(validateAddress(TWCoinTypeNano, "fake_1nanode8ngaakzbck8smq6ru9bethqwyehomf79sae1k7xd47dkidjqzffeg"));
}

TEST(Coin, ValidateAddressDGB){
    EXPECT_TRUE(validateAddress(TWCoinTypeDigiByte, "DBfCffUdSbhqKZhjuvrJ6AgvJofT4E2kp4"));
    EXPECT_TRUE(validateAddress(TWCoinTypeDigiByte, "dgb1q3p2nf26ac6qtdrv4czh5nmp2eshfj9wyn9vv3d"));
    EXPECT_TRUE(validateAddress(TWCoinTypeDigiByte, "SUngTA1vaC2E62mbnc81Mdos3TcvZHwsVo")); 
    
    // bad address
    EXPECT_FALSE(validateAddress(TWCoinTypeDigiByte, "XBfCffUdSbhqKZhjuvrJ6AgvJofT4E2kp4"));
    // bad bech32 address
    EXPECT_FALSE(validateAddress(TWCoinTypeDigiByte, "xgb1q3p2nf26ac6qtdrv4czh5nmp2eshfj9wyn9vv3d"));
    // testnet address
    EXPECT_FALSE(validateAddress(TWCoinTypeDigiByte, "ztijPBZmzdAkF6y79LHYGmqNm2CVfaoLqtz"));
}

TEST(Coin, validateAddressWanchain) {
    EXPECT_TRUE(validateAddress(TWCoinTypeWanchain, "0x9D8a62F656A8D1615c1294FD71E9cfB3e4855a4f"));
    EXPECT_FALSE(validateAddress(TWCoinTypeWanchain, "xx9D8a62F656A8D1615c1294FD71E9cfB3e4855a4f"));

    ASSERT_EQ(normalizeAddress(TWCoinTypeWanchain, "0xb08f432a3346e90e2ab61830ec227043131f70ff"), "0xB08F432a3346e90E2ab61830eC227043131F70fF");
}

TEST(Coin, validateAddressICON) {
    EXPECT_TRUE(validateAddress(TWCoinTypeICON, "hx4728fc65c31728f0d3538b8783b5394b31a136b9"));
    EXPECT_TRUE(validateAddress(TWCoinTypeICON, "cx4728fc65c31728f0d3538b8783b5394b31a136b9"));
    EXPECT_FALSE(validateAddress(TWCoinTypeICON, "xx4728fc65c31728f0d3538b8783b5394b31a136bx"));
}

TEST(Coin, validateAddressNimiq) {
    EXPECT_TRUE(validateAddress(TWCoinTypeNimiq, "NQ74 D40G N3M0 9EJD ET56 UPLR 02VC X6DU 8G1E"));
    EXPECT_FALSE(validateAddress(TWCoinTypeNimiq, "XQ74 D40G N3M0 9EJD ET56 UPLR 02VC X6DU 8G1E"));
}

TEST(Coin, validateAddressXRP) {
    EXPECT_TRUE(validateAddress(TWCoinTypeXRP, "rJHMeqKu8Ep7Fazx8MQG6JunaafBXz93YQ"));
    EXPECT_TRUE(validateAddress(TWCoinTypeXRP, "XVfvixWZQKkcenFRYApCjpTUyJ4BePTe3jJv7beatUZvQYh"));
    EXPECT_FALSE(validateAddress(TWCoinTypeXRP, "rJHMeqKu8Ep7Fazx8MQG6JunaafBXz93YQX"));
}

TEST(Coin, validateAddressKin) {
    EXPECT_TRUE(validateAddress(TWCoinTypeKin, "GDXJHJHWN6GRNOAZXON6XH74ZX6NYFAS5B7642RSJQVJTIPA4ZYUQLEB"));
    EXPECT_FALSE(validateAddress(TWCoinTypeKin, "GDXJHJHWN6GRNOAZXON6XH74ZX6NYFAS5B7642RSJQVJTIPA4ZYUQLEBX"));
}

TEST(Coin, validateAddressTron) {
    EXPECT_TRUE(validateAddress(TWCoinTypeTron, "TQLCsShbQNXMTVCjprY64qZmEA4rBarpQp"));
    EXPECT_FALSE(validateAddress(TWCoinTypeTron, "XQLCsShbQNXMTVCjprY64qZmEA4rBarpQp"));
}

TEST(Coin, validateAddressZcoin) {
    EXPECT_TRUE(validateAddress(TWCoinTypeFiro, "aHzpPjmY132KseS4nkiQTbDahTEXqesY89"));
    EXPECT_FALSE(validateAddress(TWCoinTypeFiro, "xHzpPjmY132KseS4nkiQTbDahTEXqesY89"));
}

TEST(Coin, validateAddressLitecoin) {
    EXPECT_TRUE(validateAddress(TWCoinTypeLitecoin, "ltc1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z0tamvsu"));
    EXPECT_FALSE(validateAddress(TWCoinTypeLitecoin, "xtc1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z0tamvsu"));
}

TEST(Coin, validateAddressCosmos) {
    EXPECT_TRUE(validateAddress(TWCoinTypeCosmos, "cosmos1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0emlrvp"));
    EXPECT_FALSE(validateAddress(TWCoinTypeCosmos, "cosmos1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0emlrvq"));
    EXPECT_FALSE(validateAddress(TWCoinTypeCosmos, "ltc1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z0tamvsu"));
}

TEST(Coin, validateAddressRavencoin) {
    EXPECT_TRUE(validateAddress(TWCoinTypeRavencoin, "RHoCwPc2FCQqwToYnSiAb3SrCET4zEHsbS"));
    EXPECT_TRUE(validateAddress(TWCoinTypeRavencoin, "rPWwn5h4QFZNaz1XmY39rc73sdYGGDdmq1"));

    // bad address
    EXPECT_FALSE(validateAddress(TWCoinTypeRavencoin, "XHoCwPc2FCQqwToYnSiAb3SrCET4zEHsbS"));
    // testnet address
    EXPECT_FALSE(validateAddress(TWCoinTypeRavencoin, "mwJAu1BWcRSQhepZ71wiGoSwsD6hnB5B7G"));
}

TEST(Coin, ValidateAddressWaves) {
    EXPECT_TRUE(validateAddress(TWCoinTypeWaves, "3P7WTh6kLKa8pAY4ynrSHm8TN8PVrdR7M1Q"));
    EXPECT_FALSE(validateAddress(TWCoinTypeWaves, "3P7WTh6kLKa8pAY4ynrSHm8TN8PVrdR7M1q"));
    EXPECT_FALSE(validateAddress(TWCoinTypeWaves, "2P7WTh6kLKa8pAY4ynrSHm8TN8PVrdR7M1Q"));
    EXPECT_FALSE(validateAddress(TWCoinTypeWaves, "3P7WTh6kLKa8pAY4ynrSHm8TN8PVrdR7M1Qa"));
    EXPECT_FALSE(validateAddress(TWCoinTypeWaves, "3P7WTh6kLKa8pAy4ynrSHm8TN8PVrdR7M1Q"));
}

TEST(Coin, ValidateAddressAeternity) {
    EXPECT_TRUE(validateAddress(TWCoinTypeAeternity, "ak_22ZPJTLbSDpAUfJbBJvqvXFuFe2RoP1ZEd7K9qaHJWF92MgnWm"));
    EXPECT_TRUE(validateAddress(TWCoinTypeAeternity, "ak_11111111111111111111111111111111273Yts"));

    // wrong checksum
    EXPECT_FALSE(validateAddress(TWCoinTypeAeternity, "ak_11111111111111111111111111111111273Ytt"));

    // wrong prefix
    EXPECT_FALSE(validateAddress(TWCoinTypeAeternity,"ka_11111111111111111111111111111111273Yts"));

    // wrong size
    EXPECT_FALSE(validateAddress(TWCoinTypeAeternity,"ak_111111111111111111111111111111111173Yts"));

    // empty address
    EXPECT_FALSE(validateAddress(TWCoinTypeAeternity, ""));
}

TEST(Coin, ValidateAddressTerra) {
    EXPECT_TRUE(validateAddress(TWCoinTypeTerra, "terra1krl5p2fak3nl4ph0w8tsr6y85qwczlzykv7pt7"));
    EXPECT_TRUE(validateAddress(TWCoinTypeTerra, "terravaloper1eutun6vh83lmyq0wmyf9vgghvurze2xanl9sq6"));
    EXPECT_FALSE(validateAddress(TWCoinTypeAeternity, "cosmos1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0emlrvp"));
}

TEST(Coin, ValidateAddressNebulas) {
    EXPECT_TRUE(validateAddress(TWCoinTypeNebulas, "n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY"));
    EXPECT_FALSE(validateAddress(TWCoinTypeNebulas, "a1TgpFZWCMmFd2sphb6RKsCvsEyMCNa2Yyv"));
    EXPECT_FALSE(validateAddress(TWCoinTypeNebulas, "n2TgpFZWCMmFd2sphb6RKsCvsEyMCNa2Yyv"));
    EXPECT_FALSE(validateAddress(TWCoinTypeNebulas, "123"));
}

TEST(Coin, validateAddressMonacoin) {
    EXPECT_TRUE(validateAddress(TWCoinTypeMonacoin, "MVELZC3ks1Xk59kvKWuSN3mpByNwaxeaBJ"));
    EXPECT_TRUE(validateAddress(TWCoinTypeMonacoin, "PHjTKtgYLTJ9D2Bzw2f6xBB41KBm2HeGfg"));
    EXPECT_TRUE(validateAddress(TWCoinTypeMonacoin, "mona1qp8f842ywwr9h5rdxyzggex7q3trvvvaarfssxccju52rj6htfzfsqr79j2"));

    // bad address
    EXPECT_FALSE(validateAddress(TWCoinTypeMonacoin, "YHoCwPc2FCQqwToYnSiAb3SrCET4zEHsbS"));
    // testnet address
    EXPECT_FALSE(validateAddress(TWCoinTypeMonacoin, "mwJAu1BWcRSQhepZ71wiGoSwsD6hnB5B7G"));
}

TEST(Coin, validateAddressFIO) {
    EXPECT_TRUE(validateAddress(TWCoinTypeFIO, "FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o"));

    // wrong checksum
    EXPECT_FALSE(validateAddress(TWCoinTypeFIO, "FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575q"));

    // other chain
    EXPECT_FALSE(validateAddress(TWCoinTypeFIO, "EOS65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjTF"));
    EXPECT_FALSE(validateAddress(TWCoinTypeFIO, "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2"));
}

TEST(Coin, validateAddressAlgorand) {
    EXPECT_TRUE(validateAddress(TWCoinTypeAlgorand, "ADIYK65L3XR5ODNNCUIQVEET455L56MRKJHRBX5GU4TZI2752QIWK4UL5A"));

    // wrong checksum
    EXPECT_FALSE(validateAddress(TWCoinTypeAlgorand, "ADIYK65L3XR5ODNNCUIQVEET455L56MRKJHRBX5GU4TZI2752QIWK4UL44"));

    EXPECT_FALSE(validateAddress(TWCoinTypeAlgorand, "NQ74 D40G N3M0 9EJD ET56 UPLR 02VC X6DU 8G1E"));
    EXPECT_FALSE(validateAddress(TWCoinTypeAlgorand, "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2"));
}

TEST(Coin, ValidateAddresKavaa) {
    EXPECT_TRUE(validateAddress(TWCoinTypeKava, "kava1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z09wt76x"));
    // wrong prefix
    EXPECT_FALSE(validateAddress(TWCoinTypeKava, "cosmos1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0emlrvp"));
    // wrong checksum
    EXPECT_FALSE(validateAddress(TWCoinTypeKava, "kava1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z09wt000"));
}

TEST(Coin, ValidateAddresBand) {
    EXPECT_TRUE(validateAddress(TWCoinTypeBandChain, "band1pnndgfwsrff86263xzpc5cd3t6yfvgjyqc87jh"));
    // wrong prefix
    EXPECT_FALSE(validateAddress(TWCoinTypeBandChain, "cosmos1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0emlrvp"));
    // wrong checksum
    EXPECT_FALSE(validateAddress(TWCoinTypeBandChain, "band1pnndgfwsrff86263xzpc5cd3t6yfvgjyqc8000"));
}

TEST(Coin, ValidateAddressBluzelle) {
    EXPECT_TRUE(validateAddress(TWCoinTypeBluzelle, "bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"));
    // wrong prefix
    EXPECT_FALSE(validateAddress(TWCoinTypeBluzelle, "cosmos1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"));
    // wrong checksum
    EXPECT_FALSE(validateAddress(TWCoinTypeBluzelle, "bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmunx"));
}

TEST(Coin, ValidateAddresCardano) {
    // valid V3 address
    EXPECT_TRUE(validateAddress(TWCoinTypeCardano, "addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23"));
    // valid V2 address
    EXPECT_TRUE(validateAddress(TWCoinTypeCardano, "Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W"));
    // valid V1 address
    EXPECT_TRUE(validateAddress(TWCoinTypeCardano, "DdzFFzCqrhssmYoG5Eca1bKZFdGS8d6iag1mU4wbLeYcSPVvBNF2wRG8yhjzQqErbg63N6KJA4DHqha113tjKDpGEwS5x1dT2KfLSbSJ"));
    // invalid checksum
    EXPECT_FALSE(validateAddress(TWCoinTypeCardano, "Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvm"));
    // random
    EXPECT_FALSE(validateAddress(TWCoinTypeCardano, "hasoiusaodiuhsaijnnsajnsaiussai"));
}

TEST(Coin, ValidateAddressNEO) {
    EXPECT_TRUE(validateAddress(TWCoinTypeNEO, "ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD"));
    EXPECT_FALSE(validateAddress(TWCoinTypeNEO, "hasoiusaodiuhsaijnnsajnsaiussai"));
}

TEST(Coin, ValidateAddressFilecoin) {
    EXPECT_TRUE(validateAddress(TWCoinTypeFilecoin, "f15ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7pdrq"));
    EXPECT_FALSE(validateAddress(TWCoinTypeFilecoin, "a15ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7pdrq"));
}

TEST(Coin, ValidateAddressHarmony) {
    EXPECT_TRUE(validateAddress(TWCoinTypeHarmony, "one1a50tun737ulcvwy0yvve0pvu5skq0kjargvhwe"));
    EXPECT_FALSE(validateAddress(TWCoinTypeHarmony, "oe1tp7xdd9ewwnmyvws96au0ee7e7mz6f8hjqr3g3p"));
}

TEST(Coin, ValidateAddressNEAR) {
    EXPECT_TRUE(validateAddress(TWCoinTypeNEAR, "NEAR2758Nk7CMUcxTwXdjVdSxNEidiZQWMZN3USJzj76q5ia3v2v2v"));
    EXPECT_TRUE(validateAddress(TWCoinTypeNEAR, "917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d"));
    EXPECT_FALSE(validateAddress(TWCoinTypeNEAR, "EOS65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjT"));
}

TEST(Coin, ValidateAddressSolana) {
    EXPECT_TRUE(validateAddress(TWCoinTypeSolana, "2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdST"));
    EXPECT_FALSE(validateAddress(TWCoinTypeSolana, "2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpd"));
}

TEST(Coin, ValidateAddressTheta) {
    EXPECT_TRUE(validateAddress(TWCoinTypeTheta, "0xeDe8F58dADa22c3A49dB60D4f82BAD428ab65F89"));
    EXPECT_FALSE(validateAddress(TWCoinTypeTheta, "ede8f58dada22a49db60d4f82bad428ab65f89"));

    ASSERT_EQ(normalizeAddress(TWCoinTypeTheta, "0x7d8bf18c7ce84b3e175b339c4ca93aed1dd166f1"), "0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1");
}

TEST(Coin, ValidateAddressVeChain) {
    EXPECT_TRUE(validateAddress(TWCoinTypeVeChain, "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F"));

    EXPECT_EQ(normalizeAddress(TWCoinTypeVeChain, "0x9d8a62f656a8d1615c1294fd71e9cfb3e4855a4f"), "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F");
}

TEST(Coin, ValidateAddressMultiversX) {
    EXPECT_TRUE(validateAddress(TWCoinTypeMultiversX, "erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz"));
    EXPECT_FALSE(validateAddress(TWCoinTypeMultiversX, "xerd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz"));
}

TEST(Coin, ValidateAddressOasis) {
    EXPECT_TRUE(validateAddress(TWCoinTypeOasis, "oasis1qp0cnmkjl22gky6p6qeghjytt4v7dkxsrsmueweh"));
    EXPECT_FALSE(validateAddress(TWCoinTypeOasis, "oasi1qp0cnmkjl22gky6p6qeghjytt4v7dkxsrsmueweh"));
}

TEST(Coin, ValidateAddresTHORChain) {
    EXPECT_TRUE(validateAddress(TWCoinTypeTHORChain, "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r"));
    // wrong prefix
    EXPECT_FALSE(validateAddress(TWCoinTypeTHORChain, "cosmos1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0emlrvp"));
    // wrong checksum
    EXPECT_FALSE(validateAddress(TWCoinTypeTHORChain, "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2s"));
}

TEST(Coin, ValidateAddressECash) {
    EXPECT_TRUE(validateAddress(TWCoinTypeECash, "ecash:qruxj7zq6yzpdx8dld0e9hfvt7u47zrw9gswqul42q"));
    EXPECT_TRUE(validateAddress(TWCoinTypeECash, "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2"));
    EXPECT_TRUE(validateAddress(TWCoinTypeECash, "qq07l6rr5lsdm3m80qxw80ku2ex0tj76vvft48qjaw"));
    EXPECT_TRUE(validateAddress(TWCoinTypeECash, "qqslmu0jxk4st3ldjyuazfpf5thd6vlgfu395x2elz"));

    ASSERT_EQ(normalizeAddress(TWCoinTypeECash, "qqslmu0jxk4st3ldjyuazfpf5thd6vlgfu395x2elz"), "ecash:qqslmu0jxk4st3ldjyuazfpf5thd6vlgfu395x2elz");
    ASSERT_EQ(normalizeAddress(TWCoinTypeECash, "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2"), "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2");
}

TEST(Coin, ValidateAddressEverscale) {
    EXPECT_TRUE(validateAddress(TWCoinTypeEverscale, "0:83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469a"));
    EXPECT_FALSE(validateAddress(TWCoinTypeEverscale, "83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469a"));

    ASSERT_EQ(normalizeAddress(TWCoinTypeEverscale, "0:83A0352908060FA87839195D8A763A8D9AB28F8FA41468832B398A719CC6469A"), "0:83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469a");
}

TEST(Coin, ValidateAddressNebl) {
    EXPECT_TRUE(validateAddress(TWCoinTypeNebl, "NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc"));
    EXPECT_TRUE(validateAddress(TWCoinTypeNebl, "NidLccuLD8J4oK25PwPg5ipLj5L9VVrwi5"));
}

TEST(Coin, ValidateAddressTheOpenNetwork) {
    EXPECT_TRUE(validateAddress(TWCoinTypeTON, "0:8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae"));
    EXPECT_FALSE(validateAddress(TWCoinTypeTON, "8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae"));

    ASSERT_EQ(normalizeAddress(TWCoinTypeTON, "0:8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae"), "EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorkdl");
}

} // namespace TW
