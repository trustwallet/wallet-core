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

TEST(TWOntologySignerSign, OntBalanceOf) {
    // curl  -H "Content-Type: application/json"  -X POST -d '{"Action":"sendrawtransaction",
    // "Version":"1.0.0","00d1885602ec0000000000000000000000000000000000000000000000000000000000000000000000004d1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f661400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b650000"}'
    // http://polaris2.ont.io:20334/api/v1/transaction?preExec=1
    //
    // {"Action":"sendrawtransaction","Desc":"SUCCESS","Error":0,"Result":{"State":1,"Gas":20000,"Result":"00","Notify":[]},"Version":"1.0.0"}
    auto input = Ontology::Proto::SigningInput();
    input.set_contract("ONT");
    input.set_method("balanceOf");
    input.set_query_address("ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD");
    auto data = OntTxBuilder::build(input);
    std::string serialized(TWDataBytes(data), TWDataBytes(data) + TWDataSize(data));
    auto protoOutput = Proto::SigningOutput();
    protoOutput.ParseFromString(serialized);
    auto rawTx = hex(protoOutput.encoded());
    EXPECT_EQ(0, rawTx.find("00d1"));
    EXPECT_EQ(12, rawTx.find(
                      "0000000000000000000000000000000000000000000000000000000000000000000000004"));
    EXPECT_EQ(85, rawTx.find("d1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f66"));
    EXPECT_EQ(148, rawTx.find("140000000000000000000000000000000000000001"));
    EXPECT_EQ(190, rawTx.find("0068164f6e746f6c6f67792e4e61746976652e496e766f6b650000"));
}

TEST(TWOntologySignerSign, OntDecimals) {
    // curl  -H "Content-Type: application/json"  -X POST -d '{"Action":"sendrawtransaction",
    // "Version":"1.0.0","Data":"00d1bdc12a48000000000000000000000000000000000000000000000000000000000000000000000000380008646563696d616c731400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b650000"}'
    // http://polaris2.ont.io:20334/api/v1/transaction?preExec=1
    //
    //{"Action":"sendrawtransaction","Desc":"SUCCESS","Error":0,"Result":{"State":1,"Gas":20000,"Result":"","Notify":[]},"Version":"1.0.0"}
    auto input = Ontology::Proto::SigningInput();
    input.set_contract("ONT");
    input.set_method("decimals");
    auto data = OntTxBuilder::build(input);
    std::string serialized(TWDataBytes(data), TWDataBytes(data) + TWDataSize(data));
    auto protoOutput = Proto::SigningOutput();
    protoOutput.ParseFromString(serialized);
    auto rawTx = hex(protoOutput.encoded());
    EXPECT_EQ(202, rawTx.length());
    EXPECT_EQ(0, rawTx.find("00d1"));
    EXPECT_EQ(84, rawTx.find("380008646563696d616c73"));
    EXPECT_EQ(106, rawTx.find("140000000000000000000000000000000000000001"));
}

TEST(TWOntologySignerSign, OntTransfer) {
    // tx on polaris test net.
    // https://explorer.ont.io/transaction/4a672ce813d3fac9042e9472cf9b470f8a5e59a2deb41fd7b23a1f7479a155d5/testnet
    auto ownerPrivateKey =
        parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    auto payerPrivateKey =
        parse_hex("4646464646464646464646464646464646464646464646464646464646464652");
    auto input = Ontology::Proto::SigningInput();
    input.set_contract("ONT");
    input.set_method("transfer");
    input.set_owner_private_key(ownerPrivateKey.data(), ownerPrivateKey.size());
    input.set_payer_private_key(payerPrivateKey.data(), payerPrivateKey.size());
    input.set_to_address("Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn");
    input.set_amount(1);
    input.set_gas_price(500);
    input.set_gas_limit(20000);
    auto data = OntTxBuilder::build(input);
    std::string serialized(TWDataBytes(data), TWDataBytes(data) + TWDataSize(data));
    auto protoOutput = Proto::SigningOutput();
    protoOutput.ParseFromString(serialized);
    auto rawTx = hex(protoOutput.encoded());
    EXPECT_EQ(724, rawTx.length());
    EXPECT_EQ(0, rawTx.find("00d1"));
    EXPECT_EQ(13, rawTx.find("401000000000000204e000000000000"));
    EXPECT_EQ(86, rawTx.find("00c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed29"
                             "9ea06fcb94abac41aaf3ead76586a7cc8516a7cc86c51c1087472616e73666572"));
    EXPECT_EQ(220, rawTx.find("1400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e"
                              "61746976652e496e766f6b"));
    EXPECT_EQ(452,
              rawTx.find("031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486"));
    EXPECT_EQ(656,
              rawTx.find("03d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1ee"));
}

TEST(TWOntologySignerSign, OngDecimals) {
    // curl  -H "Content-Type: application/json"  -X POST -d '{"Action":"sendrawtransaction",
    // "Version":"1.0.0","Data":"00d1e3f2e679000000000000000000000000000000000000000000000000000000000000000000000000380008646563696d616c731400000000000000000000000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b650000"}'
    // http://polaris2.ont.io:20334/api/v1/transaction?preExec=1
    //
    // {"Action":"sendrawtransaction","Desc":"SUCCESS","Error":0,"Result":{"State":1,"Gas":20000,"Result":"09","Notify":[]},"Version":"1.0.0"}
    auto input = Ontology::Proto::SigningInput();
    input.set_contract("ONG");
    input.set_method("decimals");
    auto data = OngTxBuilder::build(input);
    std::string serialized(TWDataBytes(data), TWDataBytes(data) + TWDataSize(data));
    auto protoOutput = Proto::SigningOutput();
    protoOutput.ParseFromString(serialized);
    auto rawTx = hex(protoOutput.encoded());
    EXPECT_EQ(202, rawTx.length());
    EXPECT_EQ(0, rawTx.find("00d1"));
    EXPECT_EQ(84, rawTx.find("380008646563696d616c73"));
    EXPECT_EQ(106, rawTx.find("140000000000000000000000000000000000000002"));
}

TEST(TWOntologySignerSign, OngBalanceOf) {
    // curl  -H "Content-Type: application/json"  -X POST -d '{"Action":"sendrawtransaction",
    // "Version":"1.0.0","Data":"00d1ab1ad0cf0000000000000000000000000000000000000000000000000000000000000000000000004d1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f661400000000000000000000000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b650000"}'
    // http://polaris2.ont.io:20334/api/v1/transaction?preExec=1
    //
    //{"Action":"sendrawtransaction","Desc":"SUCCESS","Error":0,"Result":{"State":1,"Gas":20000,"Result":"27e74d240609","Notify":[]},"Version":"1.0.0"}
    auto input = Ontology::Proto::SigningInput();
    input.set_contract("ONG");
    input.set_method("balanceOf");
    input.set_query_address("ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD");
    auto data = OngTxBuilder::build(input);
    std::string serialized(TWDataBytes(data), TWDataBytes(data) + TWDataSize(data));
    auto protoOutput = Proto::SigningOutput();
    protoOutput.ParseFromString(serialized);
    auto rawTx = hex(protoOutput.encoded());
    EXPECT_EQ(0, rawTx.find("00d1"));
    EXPECT_EQ(12, rawTx.find(
                      "0000000000000000000000000000000000000000000000000000000000000000000000004"));
    EXPECT_EQ(85, rawTx.find("d1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f66"));
    EXPECT_EQ(148, rawTx.find("140000000000000000000000000000000000000002"));
    EXPECT_EQ(190, rawTx.find("0068164f6e746f6c6f67792e4e61746976652e496e766f6b650000"));
}

TEST(TWOntologySignerSign, OngTransfer) {
    // tx on polaris test net.
    // https://explorer.ont.io/transaction/8a1e59396dcb72d9095088f50d1023294bf9c7b79ba693bd641578f748cbd4e6/testnet
    auto ownerPrivateKey =
        parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    auto payerPrivateKey =
        parse_hex("4646464646464646464646464646464646464646464646464646464646464652");
    auto input = Ontology::Proto::SigningInput();
    input.set_contract("ONG");
    input.set_method("transfer");
    input.set_owner_private_key(ownerPrivateKey.data(), ownerPrivateKey.size());
    input.set_payer_private_key(payerPrivateKey.data(), payerPrivateKey.size());
    input.set_to_address("Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn");
    input.set_amount(1);
    input.set_gas_price(500);
    input.set_gas_limit(20000);
    auto data = OngTxBuilder::build(input);
    std::string serialized(TWDataBytes(data), TWDataBytes(data) + TWDataSize(data));
    auto protoOutput = Proto::SigningOutput();
    protoOutput.ParseFromString(serialized);
    auto rawTx = hex(protoOutput.encoded());
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

TEST(TWOntologySignerSign, OngWithdraw) {
    // tx on polaris test net.
    // https://explorer.ont.io/transaction/433cb7ed4dec32d55be0db104aaa7ade4c7dbe0f62ef94f7b17829f7ac7cd75b/testnet
    auto ownerPrivateKey =
        parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    auto payerPrivateKey =
        parse_hex("4646464646464646464646464646464646464646464646464646464646464652");
    auto input = Ontology::Proto::SigningInput();
    input.set_contract("ONG");
    input.set_method("withdraw");
    input.set_owner_private_key(ownerPrivateKey.data(), ownerPrivateKey.size());
    input.set_payer_private_key(payerPrivateKey.data(), payerPrivateKey.size());
    input.set_to_address("AeicEjZyiXKgUeSBbYQHxsU1X3V5Buori5");
    input.set_amount(1);
    input.set_gas_price(500);
    input.set_gas_limit(20000);
    auto data = OngTxBuilder::build(input);
    std::string serialized(TWDataBytes(data), TWDataBytes(data) + TWDataSize(data));
    auto protoOutput = Proto::SigningOutput();
    protoOutput.ParseFromString(serialized);
    auto rawTx = hex(protoOutput.encoded());
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