// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/Bitcoin.pb.h"
#include "Bitcoin/Script.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;



TEST(NeblCompiler, CompileWithSignatures) {
    const auto coin = TWCoinTypeNebl;

    // tx on mainnet
    // https://Nebl-blockchain.info/tx/21314157b60ddacb842d2a749429c4112724b7a078adb9e77ba502ea2dd7c230

    const int64_t amount = 100000000;
    const int64_t fee = 226;
    const std::string toAddress = "NRrKgiZfT7jUdS3geoEBproA7hzZnDQAdr";

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc");
    input.set_coin_type(coin);
    input.set_time(1584059579);

    auto txHash0 = parse_hex("ee839754c8e93d620cbec9a1c51e7b69016d00839741b03af2c039852d941212");
    std::reverse(txHash0.begin(), txHash0.end());

    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(1);
    utxo0->mutable_out_point()->set_sequence(4294967295);
    utxo0->set_amount(20000000000000);
    
    auto script0 = TW::Bitcoin::Script::lockScriptForAddress("NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc", TWCoinTypeNebl);
    ASSERT_EQ(hex(script0.bytes), "76a914ae40b2142aba5ddd10f74d9440bfda8a36cbad5b88ac");
    utxo0->set_script(script0.bytes.data(), script0.bytes.size());

    auto utxoKey0 = PrivateKey(parse_hex("4222aae79af41eade7b07ce6fd44d926ea8e3f95e51a06e85f8bdec89680cbd9"));
    input.add_private_key(utxoKey0.bytes.data(), utxoKey0.bytes.size());

    EXPECT_EQ(input.utxo_size(), 1);
    // Plan
    Bitcoin::Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, coin);

    plan.set_amount(amount);
    plan.set_fee(fee);
    plan.set_change(19999899999774);

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
              "e7d7c67a125a506d46fa75f86a575360239d301a19621f9f231c46d889fe1a3b");

    EXPECT_EQ(hex(preSigningOutput.hash_public_keys()[0].public_key_hash()), "ae40b2142aba5ddd10f74d9440bfda8a36cbad5b");

    

    auto publicKeyHex = "03787a4c5ff72dce6d97f9b6360dc302b2d8a833e8c570dcc124a96e5f564bb524";
    auto publicKey = PublicKey(parse_hex(publicKeyHex), TWPublicKeyTypeSECP256k1);

    auto preImageHash = preSigningOutput.hash_public_keys()[0].data_hash();
    auto signature = parse_hex("3045022100a2dd4cbc8516a7b19516bce90fde7a3c4836c1277ddc61ca80d27d5711bcefc302200e049a3c2bdfd7ebacb7be48914a7cad8ea0db0695fb28ab645acdb12c413cb3");
    
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
        "01000000bbd46a5e011212942d8539c0f23ab0419783006d01697b1ec5a1c9be0c623de9c8549783ee010000006b483045022100a2dd4cbc8516a7b19516bce90fde7a3c4836c1277ddc61ca80d27d5711bcefc302200e049a3c2bdfd7ebacb7be48914a7cad8ea0db0695fb28ab645acdb12c413cb3012103787a4c5ff72dce6d97f9b6360dc302b2d8a833e8c570dcc124a96e5f564bb524ffffffff0200e1f505000000001976a914412033ed457c72ca70bab5fbfdc03256bd2ce07d88ac1e5eef96301200001976a914ae40b2142aba5ddd10f74d9440bfda8a36cbad5b88ac00000000";
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
