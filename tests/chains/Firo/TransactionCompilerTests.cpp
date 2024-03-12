// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/Bitcoin.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWCoinType.h>

#include "Bitcoin/Script.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(FiroCompiler, FiroCompileWithSignatures) {
    // tx on mainnet
    // https://explorer.firo.org/tx/f1e9a418eb8d2bc96856ac221e9112ee061805af35d52be261caf7a7c9c48756

    const auto coin = TWCoinTypeFiro;
    const int64_t amount = 9999741;
    const int64_t fee = 259;
    const std::string toAddress = "EXXQe1Xhay75BzoFFhXgpqNTtLomdBKSfyMZ";

    auto toScript = Bitcoin::Script::lockScriptForAddress(toAddress, coin);
    ASSERT_EQ(hex(toScript.bytes), "e076a9146fa0b49c4fe011eeeeba6abb9ea6832d15acda1488ac");

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("EXXWKhUtcaFKVW1NeRFuqPq33zAJMtQJwR4y");
    input.set_coin_type(coin);
    input.set_lock_time(824147);

    auto txHash0 = parse_hex("7d46af1b51ac6d55554e4748f08d87727214da7c6148da037cb71dc893b6297f");
    std::reverse(txHash0.begin(), txHash0.end());

    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(1);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX - 1);
    utxo0->set_amount(10000000);

    auto utxoAddr0 = "EXXWKhUtcaFKVW1NeRFuqPq33zAJMtQJwR4y";
    auto script0 = Bitcoin::Script::lockScriptForAddress(utxoAddr0, coin);
    ASSERT_EQ(hex(script0.bytes), "e076a914adfae82521fb6bba65fecc265fe67e5ee476b5df88ac");
    utxo0->set_script(script0.bytes.data(), script0.bytes.size());
    EXPECT_EQ(input.utxo_size(), 1);

    // Plan
    Bitcoin::Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, coin);

    plan.set_amount(amount);
    plan.set_fee(fee);
    plan.set_change(0);

    // Extend input with accepted plan
    *input.mutable_plan() = plan;

    // Serialize input
    const auto txInputData = data(input.SerializeAsString());
    EXPECT_GT((int)txInputData.size(), 0);

    /// Step 2: Obtain preimage hashes
    const auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    TW::Bitcoin::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), (int)preImageHashes.size()));

    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
    EXPECT_EQ(hex(preSigningOutput.hash_public_keys()[0].data_hash()),
              "c4841429065d36ec089c0d27b6f803b8fb1b2fb22d25629f38dcb40e2afff80d");

    EXPECT_EQ(hex(preSigningOutput.hash_public_keys()[0].public_key_hash()), "adfae82521fb6bba65fecc265fe67e5ee476b5df");

    auto publicKeyHex = "034cc1963365aa67d35643f419d6601eca6ef7f62e46bf7f8b6ffa64e2f44fd0bf";
    auto publicKey = PublicKey(parse_hex(publicKeyHex), TWPublicKeyTypeSECP256k1);
    auto preImageHash = preSigningOutput.hash_public_keys()[0].data_hash();
    auto signature = parse_hex("304402206c5135f0ebfe329b1f1ba3b53730b2e1d02a6afca9c7c9ce007b8b956f9a235a0220482e76d74375b097bcd6275ab30d0c7a716263e744ecbbc33c651f83c15c4d99");

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(
        publicKey.verifyAsDER(signature, TW::Data(preImageHash.begin(), preImageHash.end())));

    // Simulate signatures, normally obtained from signature server.
    std::vector<Data> signatureVec;
    std::vector<Data> pubkeyVec;
    signatureVec.push_back(signature);
    pubkeyVec.push_back(publicKey.bytes);

    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, signatureVec, pubkeyVec);

    const auto ExpectedTx =
        "01000000017f29b693c81db77c03da48617cda147272878df048474e55556dac511baf467d010000006a47304402206c5135f0ebfe329b1f1ba3b53730b2e1d02a6afca9c7c9ce007b8b956f9a235a0220482e76d74375b097bcd6275ab30d0c7a716263e744ecbbc33c651f83c15c4d990121034cc1963365aa67d35643f419d6601eca6ef7f62e46bf7f8b6ffa64e2f44fd0bffeffffff017d959800000000001ae076a9146fa0b49c4fe011eeeeba6abb9ea6832d15acda1488ac53930c00";
    {
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Negative: not enough signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {signature, signature}, pubkeyVec);
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {}, {});
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
    { // Negative: invalid public key
        const auto publicKeyBlake =
            parse_hex("b689ab808542e13f3d2ec56fe1efe43a1660dcadc73ce489fde7df98dd8ce5d9");
        EXPECT_EXCEPTION(
            TransactionCompiler::compileWithSignatures(
                coin, txInputData, signatureVec, {publicKeyBlake}),
            "Invalid public key");
    }
    { // Negative: wrong signature (formally valid)
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData,
            {parse_hex("415502201857bc6e6e48b46046a4bd204136fc77e24c240943fb5a1f0e86387aae59b349022"
                       "00a7f31478784e51c49f46ef072745a4f263d7efdbc9c6784aa2571ff4f6f3b51")},
            pubkeyVec);
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_signing);
    }
}
