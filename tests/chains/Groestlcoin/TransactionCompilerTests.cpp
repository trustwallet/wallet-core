// Copyright © 2017-2022 Trust Wallet.
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

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWCoinType.h>

#include "Groestlcoin/Signer.h"
#include "Groestlcoin/Transaction.h"

#include "../Bitcoin/TxComparisonHelper.h"
#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

namespace TW::Bitcoin {

TEST(GroestlcoinCompiler, CompileWithSignatures) {
    const auto coin = TWCoinTypeGroestlcoin;

    Bitcoin::Proto::SigningInput input;
    input.set_coin_type(coin);
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(2500);
    input.set_byte_fee(1);
    input.set_to_address("grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne");
    input.set_change_address("31inaRqambLsd9D7Ke4USZmGEVd3PHkh7P");

    auto utxoKey0 = parse_hex("3c3385ddc6fd95ba7282051aeb440bc75820b8c10db5c83c052d7586e3e98e84");
    input.add_private_key(utxoKey0.data(), utxoKey0.size());

    auto utxo0 = input.add_utxo();
    auto utxo0Script = Bitcoin::Script(parse_hex("76a91498af0aaca388a7e1024f505c033626d908e3b54a88ac"));
    utxo0->set_script(utxo0Script.bytes.data(), utxo0Script.bytes.size());
    utxo0->set_amount(5000);
    auto hash0 = parse_hex("9568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f");
    utxo0->mutable_out_point()->set_hash(hash0.data(), hash0.size());
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);

    Bitcoin::Proto::TransactionPlan plan;
    {
        // try plan first
        ANY_PLAN(input, plan, coin);
        EXPECT_TRUE(verifyPlan(plan, {5000}, 2500, 221));
    }

    // Supply plan for signing, to match fee of previously-created real TX
    *input.mutable_plan() = plan;
    input.mutable_plan()->set_fee(226);
    input.mutable_plan()->set_change(2274);

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
              "0fb3da786ad1028574f0b40ff1446515eb85cacccff3f3d0459e191b660597b3");

    // compile
    auto publicKey = PrivateKey(utxoKey0).getPublicKey(TWPublicKeyTypeSECP256k1);
    auto signature = parse_hex("304402202163ab98b028aa13563f0de00b785d6df81df5eac0b7c91d23f5be7ea674aa3702202bf6cd7055c6f8f697ce045b1a4f9b997cf6e5761a661d27696ac34064479d19");
    {
        const Data outputData =
            TransactionCompiler::compileWithSignatures(coin, txInputData, {signature}, {publicKey.bytes});
        Bitcoin::Proto::SigningOutput signingOutput;
        ASSERT_TRUE(signingOutput.ParseFromArray(outputData.data(), (int)outputData.size()));
        ASSERT_EQ(hex(signingOutput.encoded()),
                  "01000000019568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f000000"
                  "006a47304402202163ab98b028aa13563f0de00b785d6df81df5eac0b7c91d23f5be7ea674aa3702"
                  "202bf6cd7055c6f8f697ce045b1a4f9b997cf6e5761a661d27696ac34064479d19012103b85cc59b"
                  "67c35851eb5060cfc3a759a482254553c5857075c9e247d74d412c91ffffffff02c4090000000000"
                  "001600147557920fbc32a1ef4ef26bae5e8ce3f95abf09cee20800000000000017a9140055b0c94d"
                  "f477ee6b9f75185dfc9aa8ce2e52e48700000000");
        Bitcoin::Proto::SigningOutput output;
        ANY_SIGN(input, coin);
        ASSERT_EQ(output.encoded(), signingOutput.encoded());
    }

    { // Negative: not enough signatures
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {signature}, {publicKey.bytes, publicKey.bytes});
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
    { // Negative: empty signatures
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {}, {});
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

} // TW::Bitcoin