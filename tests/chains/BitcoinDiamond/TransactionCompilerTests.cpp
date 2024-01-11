// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
#include "Bitcoin/SegwitAddress.h"
#include "Bitcoin/TransactionInput.h"
#include "Bitcoin/TransactionPlan.h"
#include "Bitcoin/TransactionSigner.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(BitcoinDiamondCompiler, CompileWithSignatures) {
    // tx on mainnet
    // http://explorer.btcd.io/#/tx?tx=6f8db2317c0940ff97c461e5e9b89692c6c1fded15fb30ae8b9cc2429ce43f66

    const auto coin = TWCoinTypeBitcoinDiamond;
    const int64_t amount = 196007725;
    const int64_t fee = 1014;
    const std::string toAddress = "39mKL9gxk29f2RiofywHYRDmgXPv1ur8uC";

    auto toScript = Bitcoin::Script::lockScriptForAddress(toAddress, coin);
    ASSERT_EQ(hex(toScript.bytes), "a914589133651fd11901381ecb4d3beef58bc28ba2e787");

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("15ehpdrZqfZ5rj2e4T4hZKMi3kA8qdSyQu");
    input.set_coin_type(coin);

    auto txHash0 = parse_hex("6ce528c1192a9be648dd8c960695a15454c4c77b5a1dd5c8a5a208e6ae7e0ca8");
    std::reverse(txHash0.begin(), txHash0.end());

    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(501008739);

    auto utxoAddr0 = "15ehpdrZqfZ5rj2e4T4hZKMi3kA8qdSyQu";
    auto script0 = Bitcoin::Script::lockScriptForAddress(utxoAddr0, coin);
    ASSERT_EQ(hex(script0.bytes), "76a9143301f83977102415e34cccd5ca15136a3dba87d588ac");
    utxo0->set_script(script0.bytes.data(), script0.bytes.size());
    EXPECT_EQ(input.utxo_size(), 1);

    // Plan
    Bitcoin::Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, coin);

    plan.set_amount(amount);
    plan.set_fee(fee);
    plan.set_change(305000000);
    auto preBlockHash = parse_hex("840980d100724999ea20e8b14ddd5ea5e37e2beacb9157a17fe87d0854bc7e6f");
    std::reverse(preBlockHash.begin(), preBlockHash.end());
    plan.set_preblockhash(preBlockHash.data(), (int)preBlockHash.size());

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
              "13565ac8d1d5a8a721417e0391cd13ea1a212b51b9d6bba093babaa203ed9d74");

    EXPECT_EQ(hex(preSigningOutput.hash_public_keys()[0].public_key_hash()), "3301f83977102415e34cccd5ca15136a3dba87d5");

    auto publicKeyHex = "02f65e76c2a7c239bd6c8b18dc10b71d463b96c0b0d827c97345e6bbe8ee8f2ddc";
    auto publicKey = PublicKey(parse_hex(publicKeyHex), TWPublicKeyTypeSECP256k1);
    auto preImageHash = preSigningOutput.hash_public_keys()[0].data_hash();
    auto signature = parse_hex("3045022100e2c048cdf844c77275ac92cc27cfc357155d42d9a82d5d22f62247dce7681467022052c57d744a2ea91970b14e8863efdbcb3fb91f6448c027c25a8e86b752acb5ce");

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
        "0c0000006f7ebc54087de87fa15791cbea2b7ee3a55edd4db1e820ea99497200d180098401a80c7eaee608a2a5c8d51d5a7bc7c45454a19506968cdd48e69b2a19c128e56c000000006b483045022100e2c048cdf844c77275ac92cc27cfc357155d42d9a82d5d22f62247dce7681467022052c57d744a2ea91970b14e8863efdbcb3fb91f6448c027c25a8e86b752acb5ce012102f65e76c2a7c239bd6c8b18dc10b71d463b96c0b0d827c97345e6bbe8ee8f2ddcffffffff022dd7ae0b0000000017a914589133651fd11901381ecb4d3beef58bc28ba2e78740ee2d12000000001976a9143301f83977102415e34cccd5ca15136a3dba87d588ac00000000";
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
