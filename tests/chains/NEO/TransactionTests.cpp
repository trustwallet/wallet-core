// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "uint256.h"
#include "HexCoding.h"
#include "NEO/Transaction.h"
#include "NEO/TransactionType.h"
#include "NEO/TransactionAttributeUsage.h"
#include "NEO/TransactionAttribute.h"
#include <gtest/gtest.h>

namespace TW::NEO::tests {

using namespace std;

TEST(NEOTransaction, SerializeDeserializeEmpty) {
    auto transaction = Transaction();
    EXPECT_EQ(transaction, transaction);

    EXPECT_EQ(0ul, transaction.attributes.size());
    EXPECT_EQ(0ul, transaction.inInputs.size());
    EXPECT_EQ(0ul, transaction.outputs.size());
    auto serialized = transaction.serialize();

    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    EXPECT_EQ(transaction, deserializedTransaction);
}

TEST(NEOTransaction, SerializeDeserializeEmptyCollections) {
    auto transaction = Transaction();
    transaction.type = TransactionType::TT_EnrollmentTransaction;
    transaction.version = 0x07;
    const string zeroVarLong = "00";
    auto serialized = transaction.serialize();
    EXPECT_EQ("2007" + zeroVarLong + zeroVarLong + zeroVarLong, hex(serialized));

    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    EXPECT_EQ(transaction, deserializedTransaction);
    EXPECT_EQ(transaction, transaction);
}

TEST(NEOTransaction, SerializeDeserializeAttribute) {
    auto transaction = Transaction();
    transaction.type = TransactionType::TT_ContractTransaction;
    transaction.version = 0x07;
    const string zeroVarLong = "00";
    const string oneVarLong = "01";
    transaction.attributes.push_back(TransactionAttribute());
    transaction.attributes[0].usage = TransactionAttributeUsage::TAU_ContractHash;
    transaction.attributes[0]._data = parse_hex("bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a");
    auto serialized = transaction.serialize();
    EXPECT_EQ("8007" + oneVarLong + hex(transaction.attributes[0].serialize()) + zeroVarLong + zeroVarLong, hex(serialized));

    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    EXPECT_EQ(transaction, deserializedTransaction);

    transaction.attributes.push_back(TransactionAttribute());
    transaction.attributes[1].usage = TransactionAttributeUsage::TAU_ECDH02;
    transaction.attributes[1]._data = parse_hex("02b7ecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a");
    serialized = transaction.serialize();
    const string twoVarLong = "02";
    string expectedSerialized = "8007" + twoVarLong;
    expectedSerialized += hex(transaction.attributes[0].serialize());
    expectedSerialized += hex(transaction.attributes[1].serialize());
    expectedSerialized += zeroVarLong + zeroVarLong;
    EXPECT_EQ(expectedSerialized, hex(serialized));

    deserializedTransaction.deserialize(serialized);
    EXPECT_EQ(transaction, deserializedTransaction);
    EXPECT_EQ(transaction, transaction);
}

TEST(NEOTransaction, SerializeDeserializeInputs) {
    auto transaction = Transaction();
    transaction.type = TransactionType::TT_ContractTransaction;
    transaction.version = 0x07;
    const string zeroVarLong = "00";
    const string oneVarLong = "01";
    transaction.inInputs.push_back(CoinReference());
    transaction.inInputs[0].prevHash = load(parse_hex("bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.inInputs[0].prevIndex = 0xa;
    auto serialized = transaction.serialize();
    EXPECT_EQ("8007" + zeroVarLong + oneVarLong + hex(transaction.inInputs[0].serialize()) + zeroVarLong, hex(serialized));

    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    EXPECT_EQ(transaction, deserializedTransaction);

    transaction.inInputs.push_back(CoinReference());
    transaction.inInputs[1].prevHash = load(parse_hex("bdecbb623eee4f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.inInputs[1].prevIndex = 0xbc;
    serialized = transaction.serialize();
    const string twoVarLong = "02";
    string expectedSerialized = "8007" + zeroVarLong + twoVarLong;
    expectedSerialized += hex(transaction.inInputs[0].serialize());
    expectedSerialized += hex(transaction.inInputs[1].serialize());
    expectedSerialized += zeroVarLong;
    EXPECT_EQ(expectedSerialized, hex(serialized));

    deserializedTransaction.deserialize(serialized);
    EXPECT_EQ(transaction, deserializedTransaction);
    EXPECT_EQ(transaction, transaction);
}

TEST(NEOTransaction, SerializeDeserializeOutputs) {
    auto transaction = Transaction();
    transaction.type = TransactionType::TT_ContractTransaction;
    transaction.version = 0x07;
    const string zeroVarLong = "00";
    const string oneVarLong = "01";
    transaction.outputs.push_back(TransactionOutput());
    transaction.outputs[0].assetId = load(parse_hex("bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.outputs[0].scriptHash = load(parse_hex("cbb23e6f9ade28d5a8ff3eac9d73af039e821b1b"));
    transaction.outputs[0].value = 0x2;
    auto serialized = transaction.serialize();
    EXPECT_EQ("8007" + zeroVarLong + zeroVarLong + oneVarLong + hex(transaction.outputs[0].serialize()), hex(serialized));

    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    EXPECT_EQ(transaction, deserializedTransaction);

    transaction.outputs.push_back(TransactionOutput());
    transaction.outputs[1].assetId = load(parse_hex("bdecbb623eee6a9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.outputs[1].scriptHash = load(parse_hex("cbb23e6f9a3e28d5a8ff3eac9d73af039e821b1b"));
    transaction.outputs[1].value = 0x2;
    serialized = transaction.serialize();
    const string twoVarLong = "02";
    string expectedSerialized = "8007" + zeroVarLong + zeroVarLong + twoVarLong;
    expectedSerialized += hex(transaction.outputs[0].serialize());
    expectedSerialized += hex(transaction.outputs[1].serialize());
    EXPECT_EQ(expectedSerialized, hex(serialized));

    deserializedTransaction.deserialize(serialized);
    EXPECT_EQ(transaction, deserializedTransaction);
}

TEST(NEOTransaction, SerializeDeserialize) {
    auto transaction = Transaction();
    transaction.type = TransactionType::TT_ContractTransaction;
    transaction.version = 0x07;
    const string oneVarLong = "01";

    transaction.attributes.push_back(TransactionAttribute());
    transaction.attributes[0].usage = TransactionAttributeUsage::TAU_ContractHash;
    transaction.attributes[0]._data = parse_hex("bdecbb623eee6f9ade28d5a8ff5fbdea9c9d73af039e0286201b3b0291fb4d4a");

    transaction.inInputs.push_back(CoinReference());
    transaction.inInputs[0].prevHash = load(parse_hex("bdecbb623eee679ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.inInputs[0].prevIndex = 0xa;

    transaction.outputs.push_back(TransactionOutput());
    transaction.outputs[0].assetId = load(parse_hex("bdecbb623eee6f9ad328d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.outputs[0].scriptHash = load(parse_hex("cbb23e6f9ade28a5a8ff3eac9d73af039e821b1b"));
    transaction.outputs[0].value = 0x2;

    auto serialized = transaction.serialize();
    string expectedSerialized = "8007";
    expectedSerialized += oneVarLong + hex(transaction.attributes[0].serialize());
    expectedSerialized += oneVarLong + hex(transaction.inInputs[0].serialize());
    expectedSerialized += oneVarLong + hex(transaction.outputs[0].serialize());
    ASSERT_EQ(expectedSerialized, hex(serialized));

    auto deserializedTransaction = Transaction();
    deserializedTransaction.deserialize(serialized);
    EXPECT_EQ(transaction, deserializedTransaction);

    transaction.outputs.push_back(TransactionOutput());
    transaction.outputs[1].assetId = load(parse_hex("bdecbb623eee6a9a3e28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.outputs[1].scriptHash = load(parse_hex("cbb23e6f9a3e28d5a8ff3eac9da3af039e821b1b"));
    transaction.outputs[1].value = 0x2;
    serialized = transaction.serialize();
    const string twoVarLong = "02";
    expectedSerialized = "8007";
    expectedSerialized += oneVarLong + hex(transaction.attributes[0].serialize());
    expectedSerialized += oneVarLong + hex(transaction.inInputs[0].serialize());
    expectedSerialized += twoVarLong + hex(transaction.outputs[0].serialize());
    expectedSerialized += hex(transaction.outputs[1].serialize());
    EXPECT_EQ(expectedSerialized, hex(serialized));

    deserializedTransaction.deserialize(serialized);
    EXPECT_EQ(transaction, deserializedTransaction);

    transaction.inInputs.push_back(CoinReference());
    transaction.inInputs[1].prevHash = load(parse_hex("bdecbb623e3e6f9ade28d5a8ff4fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
    transaction.inInputs[1].prevIndex = 0xbc;
    transaction.inInputs.push_back(CoinReference());
    transaction.inInputs[2].prevHash = load(parse_hex("bdecbb624eee6f9ade28d5a8ff3fb3ea9c9d73af039e0286201b3b0291fb4d4a"));
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
    EXPECT_EQ(expectedSerialized, hex(serialized));

    deserializedTransaction.deserialize(serialized);
    EXPECT_EQ(transaction, deserializedTransaction);
}

TEST(NEOTransaction, SerializeDeserializeMiner) {
    string block2tn = "0000d11f7a2800000000";
    std::unique_ptr<Transaction> deserializedTransaction(Transaction::deserializeFrom(parse_hex(block2tn)));
    auto serialized = deserializedTransaction->serialize();
    std::unique_ptr<Transaction> serializedTransaction(Transaction::deserializeFrom(serialized));

    EXPECT_EQ(*deserializedTransaction, *serializedTransaction);

    string notMiner = "1000d11f7a2800000000";
    EXPECT_THROW(
        std::unique_ptr<Transaction> _deserializedTransaction(Transaction::deserializeFrom(parse_hex(notMiner))),
        std::invalid_argument);
}

TEST(NEOTransaction, SerializeDeserializeInvocation) {
    string data = "d1014f04e0aebb00148b720fabfe9cf041f9a27df4f7f1daaabf73c66414f88235a26e55cce0747ee827f39fd8167849672b53c1087472616e7366657267e345419e7377286ee5b0a39b56e30f6213ab9e4d00000000000000000220f88235a26e55cce0747ee827f39fd8167849672bf0084d65822107fcfd5200000141407d9089f957dbaf526a425f7dc494c62511989124f357364c750a3c3bff94e9f677c9cd497d9dcea3c7a1abbfa59411608736c86ef23297d0813699cd279fe27923210339fbbe7bdce6be3101fd8a1fb45df6782cd8ab76c80c884bfee14d5daafac392ac";
    std::unique_ptr<Transaction> deserializedTransaction(Transaction::deserializeFrom(parse_hex(data)));
    auto serialized = deserializedTransaction->serialize();
    std::unique_ptr<Transaction> serializedTransaction(Transaction::deserializeFrom(serialized));

    EXPECT_EQ(*deserializedTransaction, *serializedTransaction);
}

TEST(NEOTransaction, SerializeDeserializeContract) {
    string data = "80000001820e56309f4c26b32cc00025d0e35e7faa25641fa03138877478a155794f54490000019b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500e1f50500000000f88235a26e55cce0747ee827f39fd8167849672b0141401ca0aa69bccbcb97ca9423573a2c3c696422055c02e8d4ce37e2460ae88753c66144f4fbf392de6619c73ff27346f1d0a9f3d23967327d4d4f4b3b151dbb05942321025c01e1650f6bf488c318a0105ed7214c1b6a9a14cfebd9ec5da53a29723f8101ac";
    std::unique_ptr<Transaction> deserializedTransaction(Transaction::deserializeFrom(parse_hex(data)));
    auto serialized = deserializedTransaction->serialize();
    std::unique_ptr<Transaction> serializedTransaction(Transaction::deserializeFrom(serialized));

    EXPECT_EQ(*deserializedTransaction, *serializedTransaction);
}

TEST(NEOTransaction, GetHash) {
    string block2tn = "0000d11f7a2800000000";
    std::unique_ptr<Transaction> deserializedTransaction(Transaction::deserializeFrom(parse_hex(block2tn)));

    Data hash = parse_hex("8e3a32ba3a7e8bdb0ad9a2ad064713e45bd20eb0dab0d2e77df5b5ce985276d0");
    // It is flipped on the https://github.com/NeoResearch/neopt/blob/master/tests/ledger_Tests/Transaction.Test.cpp
    hash = Data(hash.rbegin(), hash.rend());

    EXPECT_EQ(hex(hash), hex(deserializedTransaction->getHash()));
}

TEST(NEOTransaction, SerializeSize) {
    auto transaction = Transaction();
    transaction.type = TransactionType::TT_EnrollmentTransaction;
    transaction.version = 0x07;
    const string zeroVarLong = "00";
    auto serialized = transaction.serialize();
    auto verSerialized = parse_hex("2007" + zeroVarLong + zeroVarLong + zeroVarLong);
    EXPECT_EQ(hex(verSerialized), hex(serialized));
    EXPECT_EQ(verSerialized, serialized);

    EXPECT_EQ(serialized.size(), static_cast<uint64_t>(transaction.size()));
}

} // namespace TW::NEO::tests
