// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bitcoin/Script.h"
#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/Bitcoin.pb.h"
#include "proto/Decred.pb.h"

#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

namespace TW::Decred::tests {

TEST(DecredCompiler, CompileWithSignatures) {
    const auto coin = TWCoinTypeDecred;

    const int64_t utxoValue = 39900000;
    const int64_t amount = 10000000;
    const int64_t fee = 100000;

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("Dsesp1V6DZDEtcq2behmBVKdYqKMdkh96hL");
    input.set_change_address("DsUoWCAxprdGNtKQqambFbTcSBgH1SHn9Gp");
    input.set_coin_type(coin);

    auto& utxo = *input.add_utxo();

    auto hash = parse_hex("fdbfe9dd703f306794a467f175be5bd9748a7925033ea1cf9889d7cf4dd11550");
    auto script = parse_hex("76a914b75fdec70b2e731795dd123ab40f918bf099fee088ac");
    auto utxoKey = parse_hex("ba005cd605d8a02e3d5dfd04234cef3a3ee4f76bfbad2722d1fb5af8e12e6764");

    utxo.set_amount(utxoValue);
    utxo.set_script(script.data(), script.size());

    auto& outpoint = *utxo.mutable_out_point();
    outpoint.set_hash(hash.data(), hash.size());
    outpoint.set_index(0);

    auto& plan = *input.mutable_plan();
    plan.set_amount(amount);
    plan.set_available_amount(utxoValue);
    plan.set_fee(fee);
    plan.set_change(utxoValue - amount - fee);
    auto& planUtxo = *plan.add_utxos();
    planUtxo = input.utxo(0);

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
              "9e4305478d1a69ee5c89a2e234d1cf270798d447d5db983b8fc3c817afddec34");

    // compile
    auto publicKey = PrivateKey(utxoKey).getPublicKey(TWPublicKeyTypeSECP256k1);
    auto signature = parse_hex("304402206ee887c9239e5fff0048674bdfff2a8cfbeec6cd4a3ccebcc12fac44b24cc5ac0220718f7c760818fde18bc5ba8457d43d5a145cc4cf13d2a5557cba9107e9f4558d");
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, {signature}, {publicKey.bytes});
    Decred::Proto::SigningOutput signingOutput;
    ASSERT_TRUE(signingOutput.ParseFromArray(outputData.data(), (int)outputData.size()));
    ASSERT_EQ(hex(signingOutput.encoded()),
              "0100000001fdbfe9dd703f306794a467f175be5bd9748a7925033ea1cf9889d7cf4dd1155000000000000000000002809698000000000000001976a914989b1aecabf1c24e213cc0f2d8a22ffee25dd4e188ac40b6c6010000000000001976a9142a194fc92e27fef9cc2b057bc9060c580cbb484888ac000000000000000001000000000000000000000000ffffffff6a47304402206ee887c9239e5fff0048674bdfff2a8cfbeec6cd4a3ccebcc12fac44b24cc5ac0220718f7c760818fde18bc5ba8457d43d5a145cc4cf13d2a5557cba9107e9f4558d0121026cc34b92cefb3a4537b3edb0b6044c04af27c01583c577823ecc69a9a21119b6");

    {
        input.add_private_key(utxoKey.data(), utxoKey.size());
        Decred::Proto::SigningOutput output;
        ANY_SIGN(input, coin);
        ASSERT_EQ(output.encoded(), signingOutput.encoded());
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {signature, signature}, {publicKey.bytes});
        Decred::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {}, {});
        Decred::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

TEST(DecredCompiler, UtxoWithTree) {
    const auto coin = TWCoinTypeDecred;

    const int64_t utxoValue = 10000000;
    const int64_t amount = 1000000;

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("Dcur2mcGjmENx4DhNqDctW5wJCVyT3Qeqkx");
    input.set_change_address("DskhnpQqQVgoSuKeyM6Unn2CEbfaenbcJBT");
    input.set_coin_type(coin);

    auto& utxo = *input.add_utxo();
 
    auto script = Bitcoin::Script::lockScriptForAddress("DskhnpQqQVgoSuKeyM6Unn2CEbfaenbcJBT", coin);
    auto hash = parse_hex("3f7b77a111634faa107c539b0c7db54e2cdbddc0c979568034aaa1ef56d2db90");
    std::reverse(hash.begin(), hash.end());
    utxo.set_amount(utxoValue);
    utxo.set_script(script.bytes.data(), script.bytes.size());

    auto& outpoint = *utxo.mutable_out_point();
    outpoint.set_hash(hash.data(), hash.size());
    outpoint.set_index(0);
    outpoint.set_sequence(UINT32_MAX);
    outpoint.set_tree(1);

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
              "cca7dcac2ac86f40037a51aeac7b6aaacf57e3304354449e140b698023b3fce7");
}

} // namespace TW::Decred::tests