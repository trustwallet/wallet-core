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
#include <TrustWalletCore/TWCoinType.h>

#include "Zen/Signer.h"
#include "Zen/TransactionBuilder.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(ZenCompiler, CompileWithSignatures) {
    const auto coin = TWCoinTypeZen;

    const int64_t amount = 200000;
    const std::string toAddress = "znma8BydGx1p7SZ17g5JMMWXqSoRSE7BNdQ";

    auto blockHash = parse_hex("000000000396ef95695b498168964e1733aca9fe47bb4f9b2851dcd0ec0edad0");
    std::reverse(blockHash.begin(), blockHash.end());
    auto blockHeight = 1163482;

    auto sblockHash = parse_hex("0000000002906dc9ef21c60d08cd03d192cba94de66095c63082d8e7e9436d40");
    std::reverse(sblockHash.begin(), sblockHash.end());
    auto sblockHeight = 1163438;

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("zncug4MEDrunR5WgdWfGB1t9Bjp8RCpKxA6");
    input.set_coin_type(coin);
    input.set_lock_time(1163772);

    auto txHash0 = parse_hex("89f799d7aaf17dbc619f5c68aa5a5ae55ceec779f9009203a87359217405f8d8");
    std::reverse(txHash0.begin(), txHash0.end());

    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(1);
    utxo0->mutable_out_point()->set_sequence(4294967294);
    utxo0->set_amount(1249057);

    auto utxoAddr0 = "znj6M9EbCmU7UKN2zgAQ8j1GwUnr4QbZBYt";
    // build utxo scriptPubKey
    // check 89f799d7aaf17dbc619f5c68aa5a5ae55ceec779f9009203a87359217405f8d8,1
    auto script0 = Bitcoin::Script::lockScriptForAddress(utxoAddr0, coin, sblockHash, sblockHeight);
    utxo0->set_script(script0.bytes.data(), script0.bytes.size());

    auto plan = Zen::TransactionBuilder::plan(input);
    ASSERT_EQ(plan.fee, 226);
    plan.preBlockHash = blockHash;
    plan.preBlockHeight = blockHeight;
    plan.fee = 302;
    plan.change = 1249057 - plan.amount - plan.fee;

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
              "882e2e61e740ff3d5889995679bf3dcda1b872e0d93be23c89a4fd4e3837f200");

    // compile
    auto publicKey = PublicKey(parse_hex("02806408d2f6d5095bb73e89f9edbe02fe81853f25c541d33da4422c6916c1d0e1"), TWPublicKeyTypeSECP256k1);
    TW::Data signature = parse_hex("3045022100b27a4d10a4c5e758c4a379ccf7050eae6d8d4dacf5c65894d024de5ab947d4640220194ffccb29c95fe0ae3fb91a40276536494102891c6c5a9aee6063106fa55d30");
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, {signature}, {publicKey.bytes});
    Bitcoin::Proto::SigningOutput signingOutput;
    ASSERT_TRUE(signingOutput.ParseFromArray(outputData.data(), (int)outputData.size()));
    // txid: 0fc555f8e205e66576f760d99270eaa6d60480c0e816209b2058387b65c2a000
    ASSERT_EQ(hex(signingOutput.encoded()), "0100000001d8f80574215973a8039200f979c7ee5ce55a5aaa685c9f61bc7df1aad799f789010000006b483045022100b27a4d10a4c5e758c4a379ccf7050eae6d8d4dacf5c65894d024de5ab947d4640220194ffccb29c95fe0ae3fb91a40276536494102891c6c5a9aee6063106fa55d30012102806408d2f6d5095bb73e89f9edbe02fe81853f25c541d33da4422c6916c1d0e1feffffff02400d0300000000003f76a914e0b858909b6b2c14996658085ed907abd880d32d88ac20d0da0eecd0dc51289b4fbb47fea9ac33174e966881495b6995ef96030000000003dac011b4b3001000000000003f76a91481b1b83b2ae8a4cddd72750dc5252c4bddd4e57e88ac20d0da0eecd0dc51289b4fbb47fea9ac33174e966881495b6995ef96030000000003dac011b4fcc11100");

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {signature, signature}, {publicKey.bytes});
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
