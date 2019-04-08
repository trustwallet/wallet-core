// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"

#include "Ontology/Ong.h"

#include <gtest/gtest.h>
#include <string>

using namespace TW;
using namespace TW::Ontology;

TEST(OntologyOng, decimals) {
    auto tx = Ong().decimals();
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ(202, rawTx.length());
    EXPECT_EQ(0, rawTx.find("00d1"));
    EXPECT_EQ(84, rawTx.find("380008646563696d616c73"));
    EXPECT_EQ(106, rawTx.find("140000000000000000000000000000000000000002"));
}

TEST(OntologyOng, balanceOf) {
    auto address = Address("ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD");
    auto tx = Ong().balanceOf(address);
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ(0, rawTx.find("00d1"));
    EXPECT_EQ(12, rawTx.find(
                      "0000000000000000000000000000000000000000000000000000000000000000000000004"));
    EXPECT_EQ(85, rawTx.find("d1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f66"));
    EXPECT_EQ(148, rawTx.find("140000000000000000000000000000000000000002"));
    EXPECT_EQ(190, rawTx.find("0068164f6e746f6c6f67792e4e61746976652e496e766f6b650000"));
}

TEST(OntologyOng, transfer) {
    auto signer1 = Signer(PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464646")));
    auto signer2 = Signer(PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464652")));
    auto toAddress = Address("Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn");
    uint64_t amount = 1;
    uint64_t gasPrice = 500;
    uint64_t gasLimit = 20000;
    auto tx = Ong().transfer(signer1, toAddress, amount, signer2, gasPrice, gasLimit);
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ(724, rawTx.length());
    EXPECT_EQ(0, rawTx.find("00d1"));
    EXPECT_EQ(13, rawTx.find("401000000000000204e000000000000"));
    EXPECT_EQ(86, rawTx.find("00c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed29"
                             "9ea06fcb94abac41aaf3ead76586a7cc8516a7cc86c51c1087472616e73666572"));
    EXPECT_EQ(220, rawTx.find("1400000000000000000000000000000000000000020068164f6e746f6c6f67792e4e"
                              "61746976652e496e766f6b"));
    EXPECT_EQ(452,
              rawTx.find("031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486"));
    EXPECT_EQ(656,
              rawTx.find("03d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1ee"));
}

TEST(OntologyOng, withdraw) {
    auto signer1 = Signer(PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464646")));
    auto signer2 = Signer(PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464652")));
    uint64_t amount = 1, gasPrice = 500, gasLimit = 20000;
    auto tx = Ong().withdraw(signer1, signer1.getAddress(), amount, signer2, gasPrice, gasLimit);
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ(776, rawTx.length());
    EXPECT_EQ(0, rawTx.find("00d1"));
    EXPECT_EQ(12, rawTx.find("f401000000000000"));
    EXPECT_EQ(28, rawTx.find("204e000000000000"));
    EXPECT_EQ(44, rawTx.find("57e9d1a61f9aafa798b6c7fbeae35639681d7df6"));
    EXPECT_EQ(61, rawTx.find("8b"));
    EXPECT_EQ(
        86, rawTx.find(
                "00c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc81400000000000000000000000000"
                "000000000000016a7cc814fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc8516a7cc86c"));
    EXPECT_EQ(246, rawTx.find("0c7472616e7366657246726f6d"));
    EXPECT_EQ(504,
              rawTx.find("031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486"));
    EXPECT_EQ(708,
              rawTx.find("03d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1ee"));
}