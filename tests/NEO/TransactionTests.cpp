// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <gtest/gtest.h>

#include "ReadData.h"
#include "UInt.hpp"
#include "HexCoding.h"
#include "NEO/Transaction.h"
#include "NEO/TransactionType.h"
#include "NEO/TransactionAttributeUsage.h"
#include "NEO/TransactionAttribute.hpp"

#include <iostream>
using namespace std;
using namespace TW;
using namespace TW::NEO;

TEST(NEOTransaction, SerializeDeserializeEmpty) {
    auto transaction = Transaction();
    ASSERT_EQ(0, transaction.attributes.size());
    ASSERT_EQ(0, transaction.inInputs.size());
    ASSERT_EQ(0, transaction.outputs.size());
    auto serialized = transaction.serialize();
    
    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    ASSERT_EQ(transaction, deserializedTransaction);
}

TEST(NEOTransaction, SerializeDeserializeEmptyCollections) {
    auto transaction = Transaction();
    transaction.type = TransactionType::TT_EnrollmentTransaction;
    transaction.version = 0x07;
    const string zeroVarLong = "00";
    auto serialized = transaction.serialize();
    ASSERT_EQ("2007" + zeroVarLong + zeroVarLong + zeroVarLong, hex(serialized));

    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    ASSERT_EQ(transaction, deserializedTransaction);
}

TEST(NEOTransaction, SerializeDeserializeAttribute) {
    auto transaction = Transaction();
    transaction.type = TransactionType::TT_ContractTransaction;
    transaction.version = 0x07;
    const string zeroVarLong = "00";
    const string oneVarLong = "01";
    transaction.attributes.push_back(TransactionAttribute());
    transaction.attributes[0].usage = TransactionAttributeUsage::TAU_ContractHash;
    transaction.attributes[0].data = parse_hex("bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a");
    auto serialized = transaction.serialize();
    ASSERT_EQ("8007" + oneVarLong + hex(transaction.attributes[0].serialize()) + zeroVarLong + zeroVarLong, hex(serialized));

    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    ASSERT_EQ(transaction, deserializedTransaction);

    transaction.attributes.push_back(TransactionAttribute());
    transaction.attributes[1].usage = TransactionAttributeUsage::TAU_ECDH02;
    transaction.attributes[1].data = parse_hex("b7ecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a");
    serialized = transaction.serialize();
    const string twoVarLong = "02";
    string expectedSerialized = "8007" + twoVarLong;
    expectedSerialized += hex(transaction.attributes[0].serialize());
    expectedSerialized += hex(transaction.attributes[1].serialize());
    expectedSerialized += zeroVarLong + zeroVarLong;
    ASSERT_EQ(expectedSerialized, hex(serialized));

    deserializedTransaction.deserialize(serialized);
    ASSERT_EQ(transaction, deserializedTransaction);
}

TEST(NEOTransaction, SerializeDeserializeInputs) {
    auto transaction = Transaction();
    transaction.type = TransactionType::TT_ContractTransaction;
    transaction.version = 0x07;
    const string zeroVarLong = "00";
    const string oneVarLong = "01";
    transaction.inInputs.push_back(CoinReference());
    transaction.inInputs[0].prevHash = load<uint256_t>(parse_hex("bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.inInputs[0].prevIndex = 0xa;
    auto serialized = transaction.serialize();
    ASSERT_EQ("8007" + zeroVarLong + oneVarLong + hex(transaction.inInputs[0].serialize()) + zeroVarLong, hex(serialized));

    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    ASSERT_EQ(transaction, deserializedTransaction);

    transaction.inInputs.push_back(CoinReference());
    transaction.inInputs[1].prevHash = load<uint256_t>(parse_hex("bdecbb623eee4f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.inInputs[1].prevIndex = 0xbc;
    serialized = transaction.serialize();
    const string twoVarLong = "02";
    string expectedSerialized = "8007" + zeroVarLong + twoVarLong;
    expectedSerialized += hex(transaction.inInputs[0].serialize());
    expectedSerialized += hex(transaction.inInputs[1].serialize());
    expectedSerialized += zeroVarLong;
    ASSERT_EQ(expectedSerialized, hex(serialized));

    deserializedTransaction.deserialize(serialized);
    ASSERT_EQ(transaction, deserializedTransaction);
}

TEST(NEOTransaction, SerializeDeserializeOutputs) {
    auto transaction = Transaction();
    transaction.type = TransactionType::TT_ContractTransaction;
    transaction.version = 0x07;
    const string zeroVarLong = "00";
    const string oneVarLong = "01";
    transaction.outputs.push_back(TransactionOutput());
    transaction.outputs[0].assetId = load<uint256_t>(parse_hex("bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.outputs[0].scriptHash = load<uint160_t>(parse_hex("cbb23e6f9ade28d5a8ff3eac9d73af039e821b1b"));
    transaction.outputs[0].value = 0x2;
    auto serialized = transaction.serialize();
    ASSERT_EQ("8007" + zeroVarLong + zeroVarLong + oneVarLong + hex(transaction.outputs[0].serialize()), hex(serialized));

    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    ASSERT_EQ(transaction, deserializedTransaction);

    transaction.outputs.push_back(TransactionOutput());
    transaction.outputs[1].assetId = load<uint256_t>(parse_hex("bdecbb623eee6a9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.outputs[1].scriptHash = load<uint160_t>(parse_hex("cbb23e6f9a3e28d5a8ff3eac9d73af039e821b1b"));
    transaction.outputs[1].value = 0x2;
    serialized = transaction.serialize();
    const string twoVarLong = "02";
    string expectedSerialized = "8007" + zeroVarLong + zeroVarLong + twoVarLong;
    expectedSerialized += hex(transaction.outputs[0].serialize());
    expectedSerialized += hex(transaction.outputs[1].serialize());
    ASSERT_EQ(expectedSerialized, hex(serialized));

    deserializedTransaction.deserialize(serialized);
    ASSERT_EQ(transaction, deserializedTransaction);
}

TEST(NEOTransaction, SerializeDeserialize) {
    auto transaction = Transaction();
    transaction.type = TransactionType::TT_ContractTransaction;
    transaction.version = 0x07;
    const string oneVarLong = "01";

    transaction.attributes.push_back(TransactionAttribute());
    transaction.attributes[0].usage = TransactionAttributeUsage::TAU_ContractHash;
    transaction.attributes[0].data = parse_hex("bdecbb623eee6f9ade28d5a8ff5fbdea9c9d73af039e0286201b3b0291fb4d4a");

    transaction.inInputs.push_back(CoinReference());
    transaction.inInputs[0].prevHash = load<uint256_t>(parse_hex("bdecbb623eee679ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.inInputs[0].prevIndex = 0xa;

    transaction.outputs.push_back(TransactionOutput());
    transaction.outputs[0].assetId = load<uint256_t>(parse_hex("bdecbb623eee6f9ad328d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.outputs[0].scriptHash = load<uint160_t>(parse_hex("cbb23e6f9ade28a5a8ff3eac9d73af039e821b1b"));
    transaction.outputs[0].value = 0x2;

    auto serialized = transaction.serialize();
    string expectedSerialized = "8007";
    expectedSerialized += oneVarLong + hex(transaction.attributes[0].serialize());
    expectedSerialized += oneVarLong + hex(transaction.inInputs[0].serialize());
    expectedSerialized += oneVarLong + hex(transaction.outputs[0].serialize());
    ASSERT_EQ(expectedSerialized, hex(serialized));

    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    ASSERT_EQ(transaction, deserializedTransaction);

    transaction.outputs.push_back(TransactionOutput());
    transaction.outputs[1].assetId = load<uint256_t>(parse_hex("bdecbb623eee6a9a3e28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.outputs[1].scriptHash = load<uint160_t>(parse_hex("cbb23e6f9a3e28d5a8ff3eac9da3af039e821b1b"));
    transaction.outputs[1].value = 0x2;
    serialized = transaction.serialize();
    const string twoVarLong = "02";
    expectedSerialized = "8007";
    expectedSerialized += oneVarLong + hex(transaction.attributes[0].serialize());
    expectedSerialized += oneVarLong + hex(transaction.inInputs[0].serialize());
    expectedSerialized += twoVarLong + hex(transaction.outputs[0].serialize());
    expectedSerialized += hex(transaction.outputs[1].serialize());
    ASSERT_EQ(expectedSerialized, hex(serialized));

    deserializedTransaction.deserialize(serialized);
    ASSERT_EQ(transaction, deserializedTransaction);

    transaction.inInputs.push_back(CoinReference());
    transaction.inInputs[1].prevHash = load<uint256_t>(parse_hex("bdecbb623e3e6f9ade28d5a8ff4fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.inInputs[1].prevIndex = 0xbc;
    transaction.inInputs.push_back(CoinReference());
    transaction.inInputs[2].prevHash = load<uint256_t>(parse_hex("bdecbb624eee6f9ade28d5a8ff3fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.inInputs[2].prevIndex = 0x1f;

    serialized = transaction.serialize();
    const string threeVarLong = "03";
    expectedSerialized = "8007";
    expectedSerialized += oneVarLong + hex(transaction.attributes[0].serialize());
    expectedSerialized += threeVarLong + hex(transaction.inInputs[0].serialize());
    expectedSerialized += hex(transaction.inInputs[1].serialize());
    expectedSerialized += hex(transaction.inInputs[2].serialize());
    expectedSerialized += twoVarLong + hex(transaction.outputs[0].serialize());
    expectedSerialized += hex(transaction.outputs[1].serialize());
    ASSERT_EQ(expectedSerialized, hex(serialized));

    deserializedTransaction.deserialize(serialized);
    ASSERT_EQ(transaction, deserializedTransaction);
}

TEST(NEOTransaction, SerializeDeserializeMiner) {
    string block2tn = "0000d11f7a2800000000";
    std::unique_ptr<Transaction> deserializedTransaction(Transaction::deserializeFrom(parse_hex(block2tn)));
    auto serialized = deserializedTransaction->serialize();
    std::unique_ptr<Transaction> serializedTransaction(Transaction::deserializeFrom(serialized));

    ASSERT_EQ(*deserializedTransaction, *serializedTransaction);
}

TEST(NEOTransaction, GetHash) {
    string block2tn = "0000d11f7a2800000000";
    std::unique_ptr<Transaction> deserializedTransaction(Transaction::deserializeFrom(parse_hex(block2tn)));

    Data hash = parse_hex("8e3a32ba3a7e8bdb0ad9a2ad064713e45bd20eb0dab0d2e77df5b5ce985276d0");
    // It is flipped on the https://github.com/NeoResearch/neopt/blob/master/tests/ledger_Tests/Transaction.Test.cpp
    hash = Data(hash.rbegin(), hash.rend());

    ASSERT_EQ(hex(hash), hex(deserializedTransaction->getHash()));
}