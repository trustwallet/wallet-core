// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <gtest/gtest.h>
#include "Zclassic/Transaction.h"
#include "Bitcoin/Script.h"
#include "Bitcoin/TransactionInput.h"
#include "Bitcoin/TransactionOutput.h"
#include "Bitcoin/OutPoint.h"
#include "Bitcoin/SignatureVersion.h"
#include "HexCoding.h"
#include "Hash.h"

namespace TW::Zclassic::tests {

using namespace TW;

class ZclassicTransactionTest : public ::testing::Test {
protected:
    Transaction createBasicTransaction() {
        auto tx = Transaction();
        tx._version = 0x80000004;  // Version 4
        tx.versionGroupId = 0x892F2085;
        tx.lockTime = 0;
        tx.expiryHeight = 0;
        tx.valueBalance = 0;
        tx.branchId = {0xbb, 0x09, 0xb8, 0x76};  // Sapling
        return tx;
    }

    Bitcoin::TransactionInput createInput(
        const std::string& prevHash = "0000000000000000000000000000000000000000000000000000000000000000",
        uint32_t index = 0,
        uint32_t sequence = 0xfffffffe
    ) {
        Bitcoin::OutPoint outPoint;
        auto hashVec = parse_hex(prevHash);
        std::copy_n(hashVec.begin(), std::min(hashVec.size(), size_t(32)), outPoint.hash.begin());
        outPoint.index = index;
        Bitcoin::TransactionInput input(outPoint, Bitcoin::Script(), sequence);
        return input;
    }

    Bitcoin::TransactionOutput createOutput(uint64_t value = 100000) {
        Bitcoin::TransactionOutput output;
        output.value = value;
        output.script = Bitcoin::Script(parse_hex("76a914" + std::string(40, '0') + "88ac"));
        return output;
    }
};

// TEST 1: Version and VersionGroupId
TEST_F(ZclassicTransactionTest, VersionAndGroupId) {
    auto tx = createBasicTransaction();
    EXPECT_EQ(tx._version, 0x80000004);
    EXPECT_EQ(tx.versionGroupId, 0x892F2085);
}

// TEST 2: Valid Branch IDs (Sapling, Blossom, Nu6)
TEST_F(ZclassicTransactionTest, ValidBranchIds) {
    auto tx = createBasicTransaction();
    
    // Sapling
    tx.branchId = {0xbb, 0x09, 0xb8, 0x76};
    EXPECT_EQ(hex(Data(tx.branchId.begin(), tx.branchId.end())), "bb09b876");

    // Blossom
    tx.branchId = {0x60, 0x0e, 0xb4, 0x2b};
    EXPECT_EQ(hex(Data(tx.branchId.begin(), tx.branchId.end())), "600eb42b");

    // Nu6
    tx.branchId = {0x55, 0x10, 0xe7, 0xc8};
    EXPECT_EQ(hex(Data(tx.branchId.begin(), tx.branchId.end())), "5510e7c8");
}

// TEST 3: getPrevoutHash
TEST_F(ZclassicTransactionTest, GetPrevoutHash) {
    auto tx = createBasicTransaction();
    tx.inputs.push_back(createInput());
    
    auto hash = tx.getPrevoutHash();
    EXPECT_EQ(hash.size(), 32);
    EXPECT_NE(hash, Data(32, 0));
}

// TEST 4: getSequenceHash
TEST_F(ZclassicTransactionTest, GetSequenceHash) {
    auto tx = createBasicTransaction();
    tx.inputs.push_back(createInput("0000000000000000000000000000000000000000000000000000000000000000", 0, 0xfffffffe));
    
    auto hash = tx.getSequenceHash();
    EXPECT_EQ(hash.size(), 32);
    EXPECT_NE(hash, Data(32, 0));
}

// TEST 5: getOutputsHash
TEST_F(ZclassicTransactionTest, GetOutputsHash) {
    auto tx = createBasicTransaction();
    tx.outputs.push_back(createOutput(50000));
    
    auto hash = tx.getOutputsHash();
    EXPECT_EQ(hash.size(), 32);
    EXPECT_NE(hash, Data(32, 0));
}

// TEST 6: getJoinSplitsHash (currently zero for transparent transactions)
TEST_F(ZclassicTransactionTest, GetJoinSplitsHash) {
    auto tx = createBasicTransaction();
    auto hash = tx.getJoinSplitsHash();
    EXPECT_EQ(hash.size(), 32);
    // For transparent-only transactions, should be zero hash
    EXPECT_EQ(hash, Data(32, 0));
}

// TEST 7: getShieldedSpendsHash (currently zero for transparent transactions)
TEST_F(ZclassicTransactionTest, GetShieldedSpendsHash) {
    auto tx = createBasicTransaction();
    auto hash = tx.getShieldedSpendsHash();
    EXPECT_EQ(hash.size(), 32);
    // For transparent-only transactions, should be zero hash
    EXPECT_EQ(hash, Data(32, 0));
}

// TEST 8: getShieldedOutputsHash (currently zero for transparent transactions)
TEST_F(ZclassicTransactionTest, GetShieldedOutputsHash) {
    auto tx = createBasicTransaction();
    auto hash = tx.getShieldedOutputsHash();
    EXPECT_EQ(hash.size(), 32);
    // For transparent-only transactions, should be zero hash
    EXPECT_EQ(hash, Data(32, 0));
}

// TEST 9: getSignatureHash with SigHashTypeAll
TEST_F(ZclassicTransactionTest, GetSignatureHashAll) {
    auto tx = createBasicTransaction();
    tx.inputs.push_back(createInput());
    tx.outputs.push_back(createOutput(50000));
    
    Bitcoin::Script scriptCode(parse_hex("76a914" + std::string(40, '0') + "88ac"));
    
    auto hash = tx.getSignatureHash(scriptCode, 0, TWBitcoinSigHashTypeAll, 100000, Bitcoin::SignatureVersion::WITNESS_V0);
    EXPECT_EQ(hash.size(), 32);
    EXPECT_NE(hash, Data(32, 0));
}

// TEST 10: getSignatureHash with AnyoneCanPay flag
TEST_F(ZclassicTransactionTest, GetSignatureHashAnyoneCanPay) {
    auto tx = createBasicTransaction();
    tx.inputs.push_back(createInput());
    tx.outputs.push_back(createOutput(50000));
    
    Bitcoin::Script scriptCode(parse_hex("76a914" + std::string(40, '0') + "88ac"));
    
    auto hashAll = tx.getSignatureHash(scriptCode, 0, TWBitcoinSigHashTypeAll, 100000, Bitcoin::SignatureVersion::WITNESS_V0);
    auto hashAnyoneCanPay = tx.getSignatureHash(
        scriptCode, 0, 
        static_cast<TWBitcoinSigHashType>(TWBitcoinSigHashTypeAll | TWBitcoinSigHashTypeAnyoneCanPay), 
        100000,
        Bitcoin::SignatureVersion::WITNESS_V0
    );
    
    // Hashes must be different when using different flags
    EXPECT_NE(hashAll, hashAnyoneCanPay);
}

// TEST 11: Transaction encoding
TEST_F(ZclassicTransactionTest, Encode) {
    auto tx = createBasicTransaction();
    tx.inputs.push_back(createInput());
    tx.outputs.push_back(createOutput(50000));
    
    Data encoded;
    tx.encode(encoded);
    
    EXPECT_GT(encoded.size(), 0);
    
    // Verify header (version 4 = 0x80000004 in little-endian)
    EXPECT_EQ(encoded[0], 0x04);
    EXPECT_EQ(encoded[1], 0x00);
    EXPECT_EQ(encoded[2], 0x00);
    EXPECT_EQ(encoded[3], 0x80);
    
    // Verify versionGroupId (0x892F2085 in little-endian)
    EXPECT_EQ(encoded[4], 0x85);
    EXPECT_EQ(encoded[5], 0x20);
    EXPECT_EQ(encoded[6], 0x2f);
    EXPECT_EQ(encoded[7], 0x89);
}

// TEST 12: Protobuf conversion
TEST_F(ZclassicTransactionTest, ProtoConversion) {
    auto tx = createBasicTransaction();
    tx.inputs.push_back(createInput());
    tx.outputs.push_back(createOutput(50000));
    
    auto protoTx = tx.proto();
    
    EXPECT_EQ(protoTx.version(), 0x80000004);
    EXPECT_EQ(protoTx.locktime(), 0);
    EXPECT_EQ(protoTx.inputs_size(), 1);
    EXPECT_EQ(protoTx.outputs_size(), 1);
}

// TEST 13: Multiple inputs and outputs
TEST_F(ZclassicTransactionTest, MultipleInputsOutputs) {
    auto tx = createBasicTransaction();
    
    tx.inputs.push_back(createInput("1111111111111111111111111111111111111111111111111111111111111111", 0));
    tx.inputs.push_back(createInput("2222222222222222222222222222222222222222222222222222222222222222", 1));
    tx.inputs.push_back(createInput("3333333333333333333333333333333333333333333333333333333333333333", 0));
    
    tx.outputs.push_back(createOutput(60000));
    tx.outputs.push_back(createOutput(40000));
    
    Data encoded;
    tx.encode(encoded);
    
    EXPECT_GT(encoded.size(), 0);
    
    auto protoTx = tx.proto();
    EXPECT_EQ(protoTx.inputs_size(), 3);
    EXPECT_EQ(protoTx.outputs_size(), 2);
}

// TEST 14: Deterministic hashing
TEST_F(ZclassicTransactionTest, DeterministicHash) {
    auto tx1 = createBasicTransaction();
    tx1.inputs.push_back(createInput());
    tx1.outputs.push_back(createOutput(50000));
    
    auto tx2 = createBasicTransaction();
    tx2.inputs.push_back(createInput());
    tx2.outputs.push_back(createOutput(50000));
    
    Bitcoin::Script scriptCode(parse_hex("76a914" + std::string(40, '0') + "88ac"));
    
    auto hash1 = tx1.getSignatureHash(scriptCode, 0, TWBitcoinSigHashTypeAll, 100000, Bitcoin::SignatureVersion::WITNESS_V0);
    auto hash2 = tx2.getSignatureHash(scriptCode, 0, TWBitcoinSigHashTypeAll, 100000, Bitcoin::SignatureVersion::WITNESS_V0);
    
    EXPECT_EQ(hash1, hash2);
}

// TEST 15: Different branch IDs produce different hashes
TEST_F(ZclassicTransactionTest, DifferentBranchIdsDifferentHashes) {
    auto tx1 = createBasicTransaction();
    tx1.branchId = {0xbb, 0x09, 0xb8, 0x76};  // Sapling
    tx1.inputs.push_back(createInput());
    tx1.outputs.push_back(createOutput(50000));
    
    auto tx2 = createBasicTransaction();
    tx2.branchId = {0x60, 0x0e, 0xb4, 0x2b};  // Blossom
    tx2.inputs.push_back(createInput());
    tx2.outputs.push_back(createOutput(50000));
    
    Bitcoin::Script scriptCode(parse_hex("76a914" + std::string(40, '0') + "88ac"));
    
    auto hashSapling = tx1.getSignatureHash(scriptCode, 0, TWBitcoinSigHashTypeAll, 100000, Bitcoin::SignatureVersion::WITNESS_V0);
    auto hashBlossom = tx2.getSignatureHash(scriptCode, 0, TWBitcoinSigHashTypeAll, 100000, Bitcoin::SignatureVersion::WITNESS_V0);
    
    EXPECT_NE(hashSapling, hashBlossom);
}

// TEST 16: ExpiryHeight and ValueBalance in encoded transaction
TEST_F(ZclassicTransactionTest, ExpiryHeightAndValueBalance) {
    auto tx = createBasicTransaction();
    tx.expiryHeight = 1000000;
    tx.valueBalance = 50000;
    
    tx.inputs.push_back(createInput());
    tx.outputs.push_back(createOutput(50000));
    
    Data encoded;
    tx.encode(encoded);
    
    EXPECT_GT(encoded.size(), 0);
}

// TEST 17: Empty transaction
TEST_F(ZclassicTransactionTest, EmptyTransaction) {
    auto tx = createBasicTransaction();
    EXPECT_TRUE(tx.empty());
    
    Data encoded;
    tx.encode(encoded);
    
    // Empty transaction should still have header and zeros
    EXPECT_GT(encoded.size(), 0);
}

// TEST 18: Non-zero lockTime
TEST_F(ZclassicTransactionTest, LockTimeNonZero) {
    auto tx = createBasicTransaction();
    tx.lockTime = 500000;
    
    tx.inputs.push_back(createInput("0000000000000000000000000000000000000000000000000000000000000000", 0, 0xfffffffe));
    tx.outputs.push_back(createOutput(50000));
    
    Data encoded;
    tx.encode(encoded);
    
    EXPECT_GT(encoded.size(), 0);
}

} // namespace TW::Zclassic::tests
