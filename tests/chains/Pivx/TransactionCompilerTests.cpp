// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/Bitcoin.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWCoinType.h>

#include "Bitcoin/Script.h"
#include "Bitcoin/TransactionInput.h"
#include "Bitcoin/TransactionPlan.h"
#include "Bitcoin/TransactionSigner.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(PivxCompiler, CompileWithSignatures) {
    // tx on mainnet
    // https://pivx.ccore.online/transaction/e5376c954c748926c373eb08df50ad72b3869be230c659689f9d83c150efd6be

    const auto coin = TWCoinTypePivx;
    const int64_t amount = 87090;
    const int64_t fee = 2910;
    const std::string toAddress = "D6MrY5B9oZaCYNaXCbt2uvmjKC1nPgrgrq";

    auto toScript = Bitcoin::Script::lockScriptForAddress(toAddress, coin);
    ASSERT_EQ(hex(toScript.bytes), "76a9140d61d810a1ae2a9c4638808dd73b64e3ea54caf488ac");

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("DFsBL73ZaDAJkzv9DeBLEzX8Jh6kkmkfzV");
    input.set_coin_type(coin);

    auto txHash0 = parse_hex("1eda07bd98ea04d322d65facaed830024e264e356810e55111cf6d7c26dff3de");
    std::reverse(txHash0.begin(), txHash0.end());

    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(1);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(26910000);

    auto utxoAddr0 = "D6E4vLacYhKc6RVxLNQg5j8rtWbAH8fybH";
    auto script0 = Bitcoin::Script::lockScriptForAddress(utxoAddr0, coin);
    ASSERT_EQ(hex(script0.bytes), "76a9140be8da968f9bc6c1c16b8c635544e757aade701388ac");
    utxo0->set_script(script0.bytes.data(), script0.bytes.size());
    EXPECT_EQ(input.utxo_size(), 1);

    // Plan
    Bitcoin::Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, coin);

    plan.set_amount(amount);
    plan.set_fee(fee);
    plan.set_change(26820000);

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
              "7d1af92dd981db6512142aa84c38385664d6751cc858a57e4adee34e6cae1449");

    EXPECT_EQ(hex(preSigningOutput.hash_public_keys()[0].public_key_hash()), "0be8da968f9bc6c1c16b8c635544e757aade7013");

    auto publicKeyHex = "0273132ffeafdc0eec3aaef5684a320a677cc77ba43df81de944af4a7fc6de66c9";
    auto publicKey = PublicKey(parse_hex(publicKeyHex), TWPublicKeyTypeSECP256k1);
    auto preImageHash = preSigningOutput.hash_public_keys()[0].data_hash();
    auto signature = parse_hex("304402202bd34158770290fb304ec85d8a92671e003681e20fb64a346ac5bd8d3686571402207c64b662d85367a949cc275a15aa10713f91815c37cf2979408dc1aa9ddbf4ab");

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
        "0100000001def3df267c6dcf1151e51068354e264e0230d8aeac5fd622d304ea98bd07da1e010000006a47304402202bd34158770290fb304ec85d8a92671e003681e20fb64a346ac5bd8d3686571402207c64b662d85367a949cc275a15aa10713f91815c37cf2979408dc1aa9ddbf4ab01210273132ffeafdc0eec3aaef5684a320a677cc77ba43df81de944af4a7fc6de66c9ffffffff0232540100000000001976a9140d61d810a1ae2a9c4638808dd73b64e3ea54caf488aca03d9901000000001976a91475a6ba23a1faaceed874538fe42362b72a1a156588ac00000000";
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
