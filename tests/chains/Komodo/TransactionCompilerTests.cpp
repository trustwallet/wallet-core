// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"
#include "Zcash/TransactionBuilder.h"

#include "proto/Bitcoin.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
namespace TW::Komodo::tests {

TEST(KomodoCompiler, CompileWithSignatures) {
    const auto coin = TWCoinTypeKomodo;

    // tx on mainnet
    // https://kmdexplorer.io/tx/dab3e7a705b0f80f0cd557a1e727dc50d8ccd24ff0ae159ca8cdefda656d7c9b
    
    const int64_t amount = 892984972;
    const int64_t fee = 407;
    const std::string toAddress = "RVUiqSDZEqTw9Ny4XRBsp6fgJKtmUj5nXD";
    auto publicKey = PublicKey(parse_hex("021f5a3a5f78b1f0adbbd8685c2c32de45e00e5b83faa814db57ce410295405207"), TWPublicKeyTypeSECP256k1);

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_coin_type(coin);

    auto txHash0 = parse_hex("f6118b221c4e5f436d536eded8486f6b0cc6ab99ca424da120fec593304acd8c");
    std::reverse(txHash0.begin(), txHash0.end());

    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(1);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(892989042);

    auto utxoAddr0 = "R9TKEwwiDLA2oD7a1jt8YmCoX2cjg1pfEU";
    auto script0 = Bitcoin::Script::lockScriptForAddress(utxoAddr0, coin);
    ASSERT_EQ(hex(script0.bytes), "76a91401ea238017d65b2c5152a81146b95582b5284c2f88ac");
    utxo0->set_script(script0.bytes.data(), script0.bytes.size());

    auto plan = Zcash::TransactionBuilder::plan(input);
    ASSERT_EQ(plan.error, Common::Proto::OK);

    plan.amount = amount;
    plan.fee = fee;
    plan.change = 0;
    std::copy(Zcash::SaplingBranchID.begin(), Zcash::SaplingBranchID.end(), std::back_inserter(plan.branchId));

    auto& protoPlan = *input.mutable_plan();
    protoPlan = plan.proto();

    // build preimage
    const auto txInputData = data(input.SerializeAsString());
    EXPECT_GT(txInputData.size(), 0ul);

    const auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    ASSERT_GT(preImageHashes.size(), 0ul);

    Bitcoin::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), int(preImageHashes.size())));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
    ASSERT_EQ(preSigningOutput.hash_public_keys().size(), 1);

    auto preImageHash = data(preSigningOutput.hash_public_keys()[0].data_hash());
    EXPECT_EQ(hex(preImageHash),
              "09323f2c24af2cf44453aa228c213f26f40e1f87548031bad35cc4c65edc087a");

    // compile
    TW::Data signature0 = parse_hex("3045022100fb6e7a940815bc0de683dd70ed85696ffe21199958161331e76954af2ba11b1b02204860632bcad9c5a3cbaa2d60c401f7616f529e4c65915f1996286d3bd54c01cb");
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, {signature0}, {publicKey.bytes});
    Bitcoin::Proto::SigningOutput signingOutput;
    ASSERT_TRUE(signingOutput.ParseFromArray(outputData.data(), (int)outputData.size()));
    ASSERT_EQ(signingOutput.error(), Common::Proto::OK);
    ASSERT_EQ(hex(signingOutput.encoded()), "0400008085202f89018ccd4a3093c5fe20a14d42ca99abc60c6b6f48d8de6e536d435f4e1c228b11f6010000006b483045022100fb6e7a940815bc0de683dd70ed85696ffe21199958161331e76954af2ba11b1b02204860632bcad9c5a3cbaa2d60c401f7616f529e4c65915f1996286d3bd54c01cb0121021f5a3a5f78b1f0adbbd8685c2c32de45e00e5b83faa814db57ce410295405207ffffffff018cde3935000000001976a914dd90c41f2916bcfea10ed11cd10ed4db01c5be6488ac00000000000000000000000000000000000000");

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {signature0, signature0}, {publicKey.bytes});
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

} // namespace TW::Komodo::tests