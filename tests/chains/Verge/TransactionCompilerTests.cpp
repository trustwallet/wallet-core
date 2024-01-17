// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/Bitcoin.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(VergeCompiler, CompileWithSignatures) {
    const auto coin = TWCoinTypeVerge;

    // tx on mainnet
    // https://verge-blockchain.info/tx/21314157b60ddacb842d2a749429c4112724b7a078adb9e77ba502ea2dd7c230

    const int64_t amount = 9999995000000;
    const int64_t fee = 120850;
    const std::string toAddress = "DQZboqURLgrBzBz4Kfbs3yV6fZ3DrNFRjQ";

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("DCUWt5ctZcPdPMYPV2o1xK1kqv7jNwxu4h");
    input.set_coin_type(coin);
    input.set_time(1584059579);

    auto txHash0 = parse_hex("ee839754c8e93d620cbec9a1c51e7b69016d00839741b03af2c039852d941212");
    std::reverse(txHash0.begin(), txHash0.end());

    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(1);
    utxo0->mutable_out_point()->set_sequence(4294967295);
    utxo0->set_amount(20000000000000);

    auto script0 = parse_hex("76a91479471b92b3c94b37544fff430556043d9acd53b188ac");
    utxo0->set_script(script0.data(), script0.size());

    EXPECT_EQ(input.utxo_size(), 1);

    // Plan
    Bitcoin::Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, coin);

    plan.set_amount(amount);
    plan.set_fee(fee);
    plan.set_change(10000004879150);

    // Extend input with accepted plan
    *input.mutable_plan() = plan;

    // Serialize input
    const auto txInputData = data(input.SerializeAsString());
    EXPECT_GT(txInputData.size(), 0ul);

    /// Step 2: Obtain preimage hashes
    const auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    TW::Bitcoin::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), (int)preImageHashes.size()));

    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
    EXPECT_EQ(hex(preSigningOutput.hash_public_keys()[0].data_hash()),
              "f7498449e2b8d33d4ff00c72b05c820e5262f43360d9f38455dcfd8f6425c9b2");

    EXPECT_EQ(hex(preSigningOutput.hash_public_keys()[0].public_key_hash()), "79471b92b3c94b37544fff430556043d9acd53b1");

    auto publicKeyHex = "02b2655122379a375a47e7a204a9dc4572cec5dbe4db4c51fea0c9fa03061fdb0b";
    auto publicKey = PublicKey(parse_hex(publicKeyHex), TWPublicKeyTypeSECP256k1);
    auto preImageHash = preSigningOutput.hash_public_keys()[0].data_hash();
    auto signature = parse_hex("3044022039e18d10ab4793d0564cfa675286d2ffd016b8f936c696fd3b72267b621dcd400220653d4761be6b12261629c4240033a08d9767a5f16851dc91a190c8a8d25ecbe0");

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
        "01000000bbd46a5e011212942d8539c0f23ab0419783006d01697b1ec5a1c9be0c623de9c8549783ee010000006a473044022039e18d10ab4793d0564cfa675286d2ffd016b8f936c696fd3b72267b621dcd400220653d4761be6b12261629c4240033a08d9767a5f16851dc91a190c8a8d25ecbe0012102b2655122379a375a47e7a204a9dc4572cec5dbe4db4c51fea0c9fa03061fdb0bffffffff02c054264e180900001976a914d50cce1f1449ac5630a0a731cbfcf7d7208a6e7d88ac2e13bd4e180900001976a91450751a6dc46f7068ac3c6350f6a85f7c20fd5e2988ac00000000";
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
}
