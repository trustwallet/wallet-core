// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "TWTestUtilities.h"

#include "../../src/Ontology/OngTxBuilder.h"
#include "../../src/Ontology/OntTxBuilder.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Ontology;

TEST(TWOntologySignerSign, OntTransfer) {
    // tx on polaris test net.
    // https://explorer.ont.io/transaction/4a672ce813d3fac9042e9472cf9b470f8a5e59a2deb41fd7b23a1f7479a155d5/testnet
    auto input = Ontology::Proto::SigningInput();
    input.set_contract("ONT");
    input.set_method("transfer");
    input.set_owner_private_key("4646464646464646464646464646464646464646464646464646464646464646");
    input.set_payer_private_key("4646464646464646464646464646464646464646464646464646464646464652");
    input.set_to_address("Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn");
    input.set_amount(1);
    input.set_gas_price(500);
    input.set_gas_limit(20000);
    auto data = OntTxBuilder::transfer(input);
    std::string serialized(TWDataBytes(data), TWDataBytes(data) + TWDataSize(data));
    auto protoOutput = Proto::SigningOutput();
    protoOutput.ParseFromString(serialized);
    auto rawTx = hex(protoOutput.encoded());
    EXPECT_EQ(724, rawTx.length());
    EXPECT_EQ(0, rawTx.find("00d1"));
    EXPECT_EQ(13, rawTx.find("401000000000000204e000000000000"));
    EXPECT_EQ(86, rawTx.find("00c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed299ea06fcb94abac41aaf3ead76586a7cc8516a7cc86c51c1087472616e73666572"));
    EXPECT_EQ(220, rawTx.find("1400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b"));
    EXPECT_EQ(452, rawTx.find("031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486"));
    EXPECT_EQ(656, rawTx.find("03d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1ee"));
}

TEST(TWOntologySignerSign, OngTransfer) {
    // tx on polaris test net.
    // https://explorer.ont.io/transaction/8a1e59396dcb72d9095088f50d1023294bf9c7b79ba693bd641578f748cbd4e6/testnet
    auto input = Ontology::Proto::SigningInput();
    input.set_contract("ONG");
    input.set_method("transfer");
    input.set_owner_private_key("4646464646464646464646464646464646464646464646464646464646464646");
    input.set_payer_private_key("4646464646464646464646464646464646464646464646464646464646464652");
    input.set_to_address("Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn");
    input.set_amount(1);
    input.set_gas_price(500);
    input.set_gas_limit(20000);
    auto data = OngTxBuilder::transfer(input);
    std::string serialized(TWDataBytes(data), TWDataBytes(data) + TWDataSize(data));
    auto protoOutput = Proto::SigningOutput();
    protoOutput.ParseFromString(serialized);
    auto rawTx = hex(protoOutput.encoded());
    EXPECT_EQ(724, rawTx.length());
    EXPECT_EQ(0, rawTx.find("00d1"));
    EXPECT_EQ(13, rawTx.find("401000000000000204e000000000000"));
    EXPECT_EQ(86, rawTx.find("00c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed299ea06fcb94abac41aaf3ead76586a7cc8516a7cc86c51c1087472616e73666572"));
    EXPECT_EQ(220, rawTx.find("1400000000000000000000000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b"));
    EXPECT_EQ(452, rawTx.find("031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486"));
    EXPECT_EQ(656, rawTx.find("03d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1ee"));
}