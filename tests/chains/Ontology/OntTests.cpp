// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ExtractVerifySign.h"
#include "HexCoding.h"
#include "Ontology/Ont.h"

#include <string>
#include <gtest/gtest.h>

namespace TW::Ontology::tests {

TEST(OntologyOnt, decimals) {
    uint32_t nonce = 0;
    auto tx = Ont().decimals(nonce);
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ("00d100000000000000000000000000000000000000000000000000000000000000000000000000000000"
              "380008646563696d616c731400000000000000000000000000000000000000010068164f6e746f6c6f67"
              "792e4e61746976652e496e766f6b650000",
              rawTx);
}

TEST(OntologyOnt, queryBalance) {
    uint32_t nonce = 0;
    auto address = Address("ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD");
    auto tx = Ont().balanceOf(address, nonce);
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ("00d100000000000000000000000000000000000000000000000000000000000000000000000000000000"
              "4d1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f6614000000000000000000"
              "00000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b650000",
              rawTx);
}

TEST(OntologyOnt, transfer) {
    PrivateKey privateKey1(parse_hex("4646464646464646464646464646464646464646464646464646464646464646"));
    Signer signer1(privateKey1);

    PrivateKey privateKey2(parse_hex("4646464646464646464646464646464646464646464646464646464646464652"));
    Signer signer2(privateKey2);

    auto toAddress = Address("Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn");
    uint32_t nonce = 0;
    uint64_t amount = 1, gasPrice = 500, gasLimit = 20000;
    auto tx = Ont().transfer(signer1, toAddress, amount, signer2, gasPrice, gasLimit, nonce);
    auto rawTx = tx.serialize();
    auto rawTxHex = hex(rawTx);

    EXPECT_EQ("00d100000000f401000000000000204e00000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df6"
              "7100c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed299ea06fcb94aba"
              "c41aaf3ead76586a7cc8516a7cc86c51c1087472616e7366657214000000000000000000000000000000"
              "00000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b65000241407531e7d5bb9ae138"
              "862585a65c26d624f1a7a61011298809d9ed9cf60d10a450bf9821152ab657ca4b7f3b1b76fb1d7021a4"
              "1d4e05d427b941caa2e9ca783afc2321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e4"
              "47125f927b7486ac4140bcc6df81d7f2f3143f152c446643ac5bf7910ef90046be8c89818264a11d360d"
              "fa89284e6d054503f6ec59833074d0717fbb23a4afaedbc98bd6f7cba2e2cf49232103d9fd62df332403"
              "d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac",
              rawTxHex);

    auto txHash = parse_hex("788562b8c1e2e1113059542025b2b810c975d2603629729da3a7220069393b38");

    size_t sign1StartsAt {160};
    EXPECT_TRUE(extractVerifySignature(privateKey1, rawTx, txHash, sign1StartsAt));

    size_t sign2StartsAt {262};
    EXPECT_TRUE(extractVerifySignature(privateKey2, rawTx, txHash, sign2StartsAt));
}

} // namespace TW::Ontology::tests
