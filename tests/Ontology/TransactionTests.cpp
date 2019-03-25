// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Ontology/Signer.h"
#include "Ontology/Transaction.h"
#include "Ontology/ParamsBuilder.h"

#include <unordered_map>

#include <PrivateKey.h>

#include <gtest/gtest.h>
#include <boost/any.hpp>

using namespace TW;
using namespace TW::Ontology;


TEST(OntologyTransaction, validity) {
    std::vector<uint8_t> ontContract{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    auto fromAddress = Address("AeicEjZyiXKgUeSBbYQHxsU1X3V5Buori5");
    auto toAddress = Address("APniYDGozkhUh8Tk7pe35aah2HGJ4fJfVd");
    uint64_t amount = 1;
    std::unordered_map<std::string, boost::any> transferParam{{"from", fromAddress.data}, {"to", toAddress.data}, {"amount", amount}};
    std::vector<boost::any> args{transferParam};
    auto invokeCode = ParamsBuilder::buildNativeInvokeCode(ontContract, 0x00, "transfer", args);
    uint8_t version = 0;
    uint8_t txType = 0xd1;
    uint32_t nonce = 1552759011;
    uint64_t gasPrice = 600;
    uint64_t gasLimit = 300000;
    auto tx = Transaction(version, txType, nonce, gasPrice, gasLimit, toAddress.string(), invokeCode);
    std::string hexTx = "00d1e3388d5c5802000000000000e09304000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df67100c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc81457e9d1a61f9aafa798b6c7fbeae35639681d7df66a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b650000";
    EXPECT_EQ(hexTx, hex(tx.serialize()));
    EXPECT_EQ(316, hex(tx.serialize()).length());
    auto signer1 = Signer("4646464646464646464646464646464646464646464646464646464646464646");
    signer1.sign(tx);
    hexTx = "00d1e3388d5c5802000000000000e09304000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df67100c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc81457e9d1a61f9aafa798b6c7fbeae35639681d7df66a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b6500014140e03a09d85f56d2ceb5817a1f3a430bab9bf0f469da38afe4a5b33de258a06236d8e0a59d25918a49825455c99f91de9caf8071e38a589a530519705af9081eca2321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486ac";
    EXPECT_EQ(520, hex(tx.serialize()).length());
    EXPECT_EQ(hexTx.substr(0, 20), hex(tx.serialize()).substr(0, 20));
    auto signer2 = Signer("4646464646464646464646464646464646464646464646464646464646464652");
    signer2.addSign(tx);
    auto result = tx.serialize();
    auto verifyPosition1 = hex(result).find("21031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486ac");
    auto verifyPosition2 = hex(result).find("2103d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac");
    EXPECT_EQ(450,verifyPosition1);
    EXPECT_EQ(654,verifyPosition2);
    EXPECT_EQ(724, hex(result).length());
}