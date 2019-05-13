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

using namespace std;
using namespace TW;
using namespace TW::NEO;

#define CHECK_VEC(F, D, VEC) { \
    ASSERT_EQ(F.VEC.size(), D.VEC.size()); \
    for (int __i_ = 0; __i_ < F.VEC.size(); ++__i_) { \
        ASSERT_EQ(hex(F.VEC[__i_].serialize()), hex(D.VEC[__i_].serialize())); \
    } \
}
#define CHECK_DES(F, D) { \
    ASSERT_EQ(transaction.type, deserializedTransaction.type); \
    ASSERT_EQ(transaction.version, deserializedTransaction.version); \
    CHECK_VEC(transaction, deserializedTransaction, attributes); \
    CHECK_VEC(transaction, deserializedTransaction, inInputs); \
    CHECK_VEC(transaction, deserializedTransaction, outputs); \
}
TEST(NEOTransaction, SerializeDeserializeEmpty) {
    auto transaction = Transaction();
    ASSERT_EQ(0, transaction.attributes.size());
    ASSERT_EQ(0, transaction.inInputs.size());
    ASSERT_EQ(0, transaction.outputs.size());
    auto serialized = transaction.serialize();
    
    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    CHECK_DES(transaction, deserializedTransaction);
}

TEST(NEOTransaction, SerializeDeserializeEmptyCollections) {
    auto transaction = Transaction();
    transaction.type = TransactionType::TT_EnrollmentTransaction;
    transaction.version = 0x07;
    const string zeroLong = "0000000000000000";
    auto serialized = transaction.serialize();
    ASSERT_EQ("2007" + zeroLong + zeroLong + zeroLong, hex(serialized));

    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    CHECK_DES(transaction, deserializedTransaction);
}

TEST(NEOTransaction, SerializeDeserializeAttribute) {
    auto transaction = Transaction();
    transaction.type = TransactionType::TT_ContractTransaction;
    transaction.version = 0x07;
    const string zeroLong = "0000000000000000";
    const string oneLong = "0100000000000000";
    transaction.attributes.push_back(TransactionAttribute());
    transaction.attributes[0].usage = TransactionAttributeUsage::TAU_ContractHash;
    transaction.attributes[0].data = parse_hex("bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a");
    auto serialized = transaction.serialize();
    ASSERT_EQ("8007" + oneLong + hex(transaction.attributes[0].serialize()) + zeroLong + zeroLong, hex(serialized));

    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    CHECK_DES(transaction, deserializedTransaction);

    transaction.attributes.push_back(TransactionAttribute());
    transaction.attributes[1].usage = TransactionAttributeUsage::TAU_ECDH02;
    transaction.attributes[1].data = parse_hex("b7ecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a");
    serialized = transaction.serialize();
    const string twoLong = "0200000000000000";
    string expectedSerialized = "8007" + twoLong;
    expectedSerialized += hex(transaction.attributes[0].serialize());
    expectedSerialized += hex(transaction.attributes[1].serialize());
    expectedSerialized += zeroLong + zeroLong;
    ASSERT_EQ(expectedSerialized, hex(serialized));

    deserializedTransaction.deserialize(serialized);
    CHECK_DES(transaction, deserializedTransaction);
}

TEST(NEOTransaction, SerializeDeserializeInputs) {
    auto transaction = Transaction();
    transaction.type = TransactionType::TT_ContractTransaction;
    transaction.version = 0x07;
    const string zeroLong = "0000000000000000";
    const string oneLong = "0100000000000000";
    transaction.inInputs.push_back(CoinReference());
    transaction.inInputs[0].prevHash = load<uint256_t>(parse_hex("bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.inInputs[0].prevIndex = 0xa;
    auto serialized = transaction.serialize();
    ASSERT_EQ("8007" + zeroLong + oneLong + hex(transaction.inInputs[0].serialize()) + zeroLong, hex(serialized));

    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    CHECK_DES(transaction, deserializedTransaction);

    transaction.inInputs.push_back(CoinReference());
    transaction.inInputs[1].prevHash = load<uint256_t>(parse_hex("bdecbb623eee4f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.inInputs[1].prevIndex = 0xbc;
    serialized = transaction.serialize();
    const string twoLong = "0200000000000000";
    string expectedSerialized = "8007" + zeroLong + twoLong;
    expectedSerialized += hex(transaction.inInputs[0].serialize());
    expectedSerialized += hex(transaction.inInputs[1].serialize());
    expectedSerialized += zeroLong;
    ASSERT_EQ(expectedSerialized, hex(serialized));

    deserializedTransaction.deserialize(serialized);
    CHECK_DES(transaction, deserializedTransaction);
}

TEST(NEOTransaction, SerializeDeserializeOutputs) {
    auto transaction = Transaction();
    transaction.type = TransactionType::TT_ContractTransaction;
    transaction.version = 0x07;
    const string zeroLong = "0000000000000000";
    const string oneLong = "0100000000000000";
    transaction.outputs.push_back(TransactionOutput());
    transaction.outputs[0].assetId = load<uint256_t>(parse_hex("bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.outputs[0].scriptHash = load<uint160_t>(parse_hex("cbb23e6f9ade28d5a8ff3eac9d73af039e821b1b"));
    transaction.outputs[0].value = 0x2;
    auto serialized = transaction.serialize();
    ASSERT_EQ("8007" + zeroLong + zeroLong + oneLong + hex(transaction.outputs[0].serialize()), hex(serialized));

    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    CHECK_DES(transaction, deserializedTransaction);

    transaction.outputs.push_back(TransactionOutput());
    transaction.outputs[1].assetId = load<uint256_t>(parse_hex("bdecbb623eee6a9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.outputs[1].scriptHash = load<uint160_t>(parse_hex("cbb23e6f9a3e28d5a8ff3eac9d73af039e821b1b"));
    transaction.outputs[1].value = 0x2;
    serialized = transaction.serialize();
    const string twoLong = "0200000000000000";
    string expectedSerialized = "8007" + zeroLong + zeroLong + twoLong;
    expectedSerialized += hex(transaction.outputs[0].serialize());
    expectedSerialized += hex(transaction.outputs[1].serialize());
    ASSERT_EQ(expectedSerialized, hex(serialized));

    deserializedTransaction.deserialize(serialized);
    CHECK_DES(transaction, deserializedTransaction);
}

TEST(NEOTransaction, SerializeDeserialize) {
    auto transaction = Transaction();
    transaction.type = TransactionType::TT_ContractTransaction;
    transaction.version = 0x07;
    const string oneLong = "0100000000000000";

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
    expectedSerialized += oneLong + hex(transaction.attributes[0].serialize());
    expectedSerialized += oneLong + hex(transaction.inInputs[0].serialize());
    expectedSerialized += oneLong + hex(transaction.outputs[0].serialize());
    ASSERT_EQ(expectedSerialized, hex(serialized));

    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    CHECK_DES(transaction, deserializedTransaction);

    transaction.outputs.push_back(TransactionOutput());
    transaction.outputs[1].assetId = load<uint256_t>(parse_hex("bdecbb623eee6a9a3e28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.outputs[1].scriptHash = load<uint160_t>(parse_hex("cbb23e6f9a3e28d5a8ff3eac9da3af039e821b1b"));
    transaction.outputs[1].value = 0x2;
    serialized = transaction.serialize();
    const string twoLong = "0200000000000000";
    expectedSerialized = "8007";
    expectedSerialized += oneLong + hex(transaction.attributes[0].serialize());
    expectedSerialized += oneLong + hex(transaction.inInputs[0].serialize());
    expectedSerialized += twoLong + hex(transaction.outputs[0].serialize());
    expectedSerialized += hex(transaction.outputs[1].serialize());
    ASSERT_EQ(expectedSerialized, hex(serialized));

    deserializedTransaction.deserialize(serialized);
    CHECK_DES(transaction, deserializedTransaction);

    transaction.inInputs.push_back(CoinReference());
    transaction.inInputs[1].prevHash = load<uint256_t>(parse_hex("bdecbb623e3e6f9ade28d5a8ff4fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.inInputs[1].prevIndex = 0xbc;
    transaction.inInputs.push_back(CoinReference());
    transaction.inInputs[2].prevHash = load<uint256_t>(parse_hex("bdecbb624eee6f9ade28d5a8ff3fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.inInputs[2].prevIndex = 0x1f;

    serialized = transaction.serialize();
    const string threeLong = "0300000000000000";
    expectedSerialized = "8007";
    expectedSerialized += oneLong + hex(transaction.attributes[0].serialize());
    expectedSerialized += threeLong + hex(transaction.inInputs[0].serialize());
    expectedSerialized += hex(transaction.inInputs[1].serialize());
    expectedSerialized += hex(transaction.inInputs[2].serialize());
    expectedSerialized += twoLong + hex(transaction.outputs[0].serialize());
    expectedSerialized += hex(transaction.outputs[1].serialize());
    ASSERT_EQ(expectedSerialized, hex(serialized));

    deserializedTransaction.deserialize(serialized);
    CHECK_DES(transaction, deserializedTransaction);
}
#undef CHECK_VEC
#undef CHECK_DES

TEST(NEOTransaction, GetHash) {
    string block2tn = "0000d11f7a2800000000";
    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(parse_hex(block2tn));

    ASSERT_EQ("0x8e3a32ba3a7e8bdb0ad9a2ad064713e45bd20eb0dab0d2e77df5b5ce985276d0", hex(deserializedTransaction.getHash()));
}
