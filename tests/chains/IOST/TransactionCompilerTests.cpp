// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Base58.h"
#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/IOST.pb.h"
#include "proto/Theta.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(IostCompiler, CompileWithSignatures) {
    const auto coin = TWCoinTypeIOST;
    /// Step 1: Prepare transaction input (protobuf)
    const auto privKeyBytes = Base58::decode(
        "4TQwN7wWXg26ByuU5WkUPErd5v6PD6HsDuULyGNJgpS979wXF7jRU8NKviJs5boHrRKbLMomKycbek4NyDy6cLb8");
    const auto pkFrom = PrivateKey(Data(privKeyBytes.begin(), privKeyBytes.begin() + 32));
    const auto publicKey = pkFrom.getPublicKey(TWPublicKeyTypeED25519);
    TW::IOST::Proto::SigningInput input;
    input.set_transfer_memo("");
    auto t = input.mutable_transaction_template();
    t->set_publisher("astastooo");
    t->set_time(1647421579901555000);
    t->set_expiration(1647421879901555000);
    t->set_gas_ratio(1);
    t->set_gas_limit(100000);
    t->set_chain_id(1023);
    t->set_delay(0);
    t->set_publisher("astastooo");
    t->add_actions();
    auto action = t->mutable_actions(0);
    action->set_contract("token.iost");
    action->set_action_name("transfer");
    action->set_data("[\"IOST\",\"astastooo\",\"test_iosted\",\"0.001\",\"test\"]");
    t->add_amount_limit();
    auto amountLimit = t->mutable_amount_limit(0);
    amountLimit->set_token("*");
    amountLimit->set_value("unlimited");
    input.mutable_account()->set_active_key(privKeyBytes.data(), 32);

    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash),
              "451ed1e542da2422ed152bff6f30c95e2a8ee2153f4d36f15c45914fa2d2e9f1");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = publicKey.bytes;
    const auto signature =
        parse_hex("1e5e2de66512658e9317fa56766678166abcf492d020863935723db2030f736710e13437cef0981f"
                  "cc376eae45349759508767d407b6c9963712910ada2c3606");

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preSigningOutput.data_hash())));

    /// Step 3: Compile transaction info
    const auto expectedTx =
        "7b2274696d65223a2231363437343231353739393031353535303030222c2265787069726174696f6e223a2231"
        "363437343231383739393031353535303030222c226761735f726174696f223a312c226761735f6c696d697422"
        "3a3130303030302c22636861696e5f6964223a313032332c22616374696f6e73223a5b7b22636f6e7472616374"
        "223a22746f6b656e2e696f7374222c22616374696f6e5f6e616d65223a227472616e73666572222c2264617461"
        "223a225b5c22494f53545c222c5c226173746173746f6f6f5c222c5c22746573745f696f737465645c222c5c22"
        "302e3030315c222c5c22746573745c225d227d5d2c22616d6f756e745f6c696d6974223a5b7b22746f6b656e22"
        "3a222a222c2276616c7565223a22756e6c696d69746564227d5d2c227075626c6973686572223a226173746173"
        "746f6f6f222c227075626c69736865725f73696773223a5b7b22616c676f726974686d223a322c227369676e61"
        "74757265223a22486c3474356d55535a593654462f7057646d5a34466d7138394a4c51494959354e5849397367"
        "4d5063326351345451337a76435948387733627135464e4a645a5549646e31416532795a59334570454b326977"
        "3242673d3d222c227075626c69635f6b6579223a2234687a496d2b6d383234536f663866645641474545332b64"
        "667931554c7a69786e6f4c5047694a565879383d227d5d7d";
    {
        const Data outputData =
            TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKeyData});

        IOST::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(hex(output.encoded()), expectedTx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::IOST::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));

        TW::IOST::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), expectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {publicKey.bytes});
        IOST::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: invalid signatures
        const auto invalidSignature =
            parse_hex("fb43727477caaa12542b9060856816d42eedef6ebf2e98e4f8dff4355fe384751925833c4a26b2fed1707aebe655cb3317504a61ee59697c086f7baa6ca06a09");
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {invalidSignature}, {publicKey.bytes});
        IOST::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_internal);
        EXPECT_EQ(output.error_message(), "Invalid signature/hash/publickey combination");
    }

    { // Negative: empty signatures
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {}, {});
        IOST::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}
