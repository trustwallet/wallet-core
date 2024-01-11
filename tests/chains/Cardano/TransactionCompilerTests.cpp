// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"
#include "uint256.h"

#include "proto/Cardano.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(CardanoCompiler, CompileWithSignaturesAndPubKeyType) {
    /// Prepare transaction input (protobuf)
    const auto coin = TWCoinTypeCardano;
    auto input = Cardano::Proto::SigningInput();
    input.mutable_transfer_message()->set_to_address("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
    input.mutable_transfer_message()->set_change_address("addr1v8mv75d2evhr4kt048cx7m3f97x363ajadha8xv8dp96nuggpv8rn");
    input.mutable_transfer_message()->set_amount(1850000);

    auto* utxo1 = input.add_utxos();
    utxo1->set_address("addr1v8mv75d2evhr4kt048cx7m3f97x363ajadha8xv8dp96nuggpv8rn");
    utxo1->set_amount(1000000);
    auto txHash = parse_hex("d87f6e99c8d3a0fb22b1ea4de477f5a6d1f0e419450c2a194304371cada0ebb9");
    utxo1->mutable_out_point()->set_tx_hash(txHash.data(), txHash.size());
    utxo1->mutable_out_point()->set_output_index(0);

    auto* utxo2 = input.add_utxos();
    utxo2->set_address("addr1v8mv75d2evhr4kt048cx7m3f97x363ajadha8xv8dp96nuggpv8rn");
    utxo2->set_amount(4040957);
    utxo2->mutable_out_point()->set_tx_hash(txHash.data(), txHash.size());
    utxo2->mutable_out_point()->set_output_index(1);

    auto* tokenBundle1 = utxo2->add_token_amount();
    tokenBundle1->set_policy_id("122d15a15dc753d2b3ca9ee46c1c6ca41dda38d735942d9d259c785b");
    tokenBundle1->set_asset_name_hex("5454546f6b656e2d31");
    const auto tokenAmount1 = store(uint256_t(3000000));
    tokenBundle1->set_amount(tokenAmount1.data(), tokenAmount1.size());

    auto* tokenBundle2 = utxo2->add_token_amount();
    tokenBundle2->set_policy_id("122d15a15dc753d2b3ca9ee46c1c6ca41dda38d735942d9d259c785b");
    tokenBundle2->set_asset_name("TTToken-2");
    const auto tokenAmount2 = store(uint256_t(3000000));
    tokenBundle2->set_amount(tokenAmount2.data(), tokenAmount2.size());

    auto* tokenBundle3 = utxo2->add_token_amount();
    tokenBundle3->set_policy_id("122d15a15dc753d2b3ca9ee46c1c6ca41dda38d735942d9d259c785b");
    tokenBundle3->set_asset_name_hex("5454546f6b656e2d33");
    const auto tokenAmount3 = store(uint256_t(5000000));
    tokenBundle3->set_amount(tokenAmount3.data(), tokenAmount3.size());

    auto inputData = data(input.SerializeAsString());
    const auto preImageHash = TransactionCompiler::preImageHashes(coin, inputData);

    auto preOut = TxCompiler::Proto::PreSigningOutput();
    preOut.ParseFromArray(preImageHash.data(), (int)preImageHash.size());
    EXPECT_EQ(hex(preOut.data_hash()), "3e5a7c1d1afbc7e3ca783daba1beb12010fc4ecc748722558697509212c9f186");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = parse_hex("17c55d712152ccabf28215fe2d008d615f94796e098a97f1aa43d986ac3cb946");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);
    const auto sig = parse_hex("1096ddcfb2ad21a4c0d861ef3fabe18841e8de88105b0d8e36430d7992c588634ead4100c32b2800b31b65e014d54a8238bdda63118d829bf0bcf1b631e86f0e");

    /// Compile transaction info
    const auto expectedTx =
        "83a40082825820d87f6e99c8d3a0fb22b1ea4de477f5a6d1f0e419450c2a194304371cada0ebb901"
        "825820d87f6e99c8d3a0fb22b1ea4de477f5a6d1f0e419450c2a194304371cada0ebb90001828258"
        "39018d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cd"
        "d9fdb03e10b4e4ac08f5da1fdec6222a34681a001c3a9082581d61f6cf51aacb2e3ad96fa9f06f6e"
        "292f8d1d47b2eb6fd39987684ba9f1821a002e0feea1581c122d15a15dc753d2b3ca9ee46c1c6ca4"
        "1dda38d735942d9d259c785ba3495454546f6b656e2d311a002dc6c0495454546f6b656e2d321a00"
        "2dc6c0495454546f6b656e2d331a004c4b40021a0002a0bf0300a1008182582017c55d712152ccab"
        "f28215fe2d008d615f94796e098a97f1aa43d986ac3cb94658401096ddcfb2ad21a4c0d861ef3fab"
        "e18841e8de88105b0d8e36430d7992c588634ead4100c32b2800b31b65e014d54a8238bdda63118d"
        "829bf0bcf1b631e86f0ef6";
    const Data outData = TransactionCompiler::compileWithSignaturesAndPubKeyType(coin, inputData, {sig}, {publicKeyData}, TWPublicKeyTypeED25519);

    {
        auto output = Cardano::Proto::SigningOutput();
        output.ParseFromArray(outData.data(), (int)outData.size());

        EXPECT_EQ(hex(output.encoded()), expectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        const Data outputData = TransactionCompiler::compileWithSignaturesAndPubKeyType(
            coin, inputData, {sig, sig}, {publicKeyData}, TWPublicKeyTypeED25519);
        Cardano::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        const Data outputData = TransactionCompiler::compileWithSignaturesAndPubKeyType(
            coin, inputData, {}, {}, TWPublicKeyTypeED25519);
        Cardano::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}
