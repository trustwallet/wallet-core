// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Ontology/Signer.h"
#include "Ontology/Transaction.h"
#include "Ontology/ParamsBuilder.h"

#include <iostream>
#include <unordered_map>

#include <PrivateKey.h>

#include <gtest/gtest.h>
#include <boost/any.hpp>

using namespace TW;
using namespace TW::Ontology;


TEST(OntologyTransaction, validity) {
    std::vector<uint8_t> ontContract{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    auto fromAddress = Address("ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD");
    auto toAddress = Address("Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn");
    uint64_t amount = 1;
    std::unordered_map<std::string, boost::any> transferParam{{"from", fromAddress.data}, {"to", toAddress.data}, {"amount", amount}};
    std::vector<boost::any> args{transferParam};
    auto invokeCode = ParamsBuilder::buildNativeInvokeCode(ontContract, 0x00, "transfer", args);
    uint8_t version = 0;
    uint8_t txType = 0xd1;
    uint32_t nonce = 1552759011;
    uint64_t gasPrice = 500;
    uint64_t gasLimit = 20000;
    auto tx = Transaction(version, txType, nonce, gasPrice, gasLimit, toAddress.string(), invokeCode);
    std::string hexTx = "00d1e3388d5cf401000000000000204e000000000000feec06b79ed299ea06fcb94abac41aaf3ead76587100c66b1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b76a7cc814feec06b79ed299ea06fcb94abac41aaf3ead76586a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b650000";
    EXPECT_EQ(hexTx, hex(tx.serialize()));

    auto signer1 = Signer("4646464646464646464646464646464646464646464646464646464646464646");
    tx.sign(signer1);
    hexTx = "00d1e3388d5cf401000000000000204e000000000000feec06b79ed299ea06fcb94abac41aaf3ead76587100c66b1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b76a7cc814feec06b79ed299ea06fcb94abac41aaf3ead76586a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b65000141401c39e8e8b7ee666a2e924a9802a70f690c4c497563e2df54ed891431657b6e39317232edb3087d5584821c9911b3e946aa445945b264314e503d765a330399392321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486ac";
    EXPECT_EQ(520, hex(tx.serialize()).length());
    EXPECT_EQ(hexTx.substr(0, 20), hex(tx.serialize()).substr(0, 20));

    auto signer2 = Signer("4646464646464646464646464646464646464646464646464646464646464652");
    tx.addSign(signer2);
    auto result = tx.serialize();
    auto verifyPosition1 = hex(result).find("21031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486ac");
    auto verifyPosition2 = hex(result).find("2103d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac");
    EXPECT_EQ(450,verifyPosition1);
    EXPECT_EQ(654,verifyPosition2);
    EXPECT_EQ(724, hex(result).length());
}