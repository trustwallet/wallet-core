// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/Stellar.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWStellarPassphrase.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(StellarCompiler, CompileWithSignatures) {
    const auto coin = TWCoinTypeStellar;
    /// Step 1: Prepare transaction input (protobuf)
    TW::Stellar::Proto::SigningInput input;
    auto privateKey =
        PrivateKey(parse_hex("59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722"));
    auto publicKey = privateKey.getPublicKey(::publicKeyType(coin));

    input.set_passphrase(TWStellarPassphrase_Stellar);
    input.set_account("GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI");
    input.set_fee(1000);
    input.set_sequence(2);
    input.mutable_op_payment()->set_destination(
        "GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52");
    input.mutable_op_payment()->set_amount(10000000);
    auto &memoText = *input.mutable_memo_text();
    memoText.set_text("Hello, world!");

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
              "1e8786a0162630b2393e0f6c51f16a2d7860715023cb19bf25cad14490b1f8f3");

    auto signature = parse_hex("5042574491827aaccbce1e2964c05098caba06194beb35e595aabfec9f788516a83"
                               "3f755f18144f4a2eedb3123d180f44e7c16037d00857c5c5b7033ebac2c01");

    /// Step 3: Compile transaction info
    const auto tx = "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/"
                    "DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAEAAAANSGVsbG8sIHdvcmxkIQAAAAAAAAEAAAAAAAAA"
                    "AQAAAADFgLYxeg6zm/"
                    "f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAAAAAAJiWgAAAAAAAAAABGd9cogAAAEBQQldEkYJ6"
                    "rMvOHilkwFCYyroGGUvrNeWVqr/sn3iFFqgz91XxgUT0ou7bMSPRgPROfBYDfQCFfFxbcDPrrCwB";
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {});
    {
        TW::Stellar::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.signature(), tx);
    }
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::Stellar::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        signingInput.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

        TW::Stellar::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.signature(), tx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {});
        Stellar::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.signature().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_signatures_count);
    }
}

TEST(StellarCompiler, PreImageHashes_MemoHash_InvalidSize) {
    const auto coin = TWCoinTypeStellar;
    TW::Stellar::Proto::SigningInput input;
    input.set_passphrase(TWStellarPassphrase_Stellar);
    input.set_account("GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI");
    input.set_fee(1000);
    input.set_sequence(2);
    input.mutable_op_payment()->set_destination(
        "GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52");
    input.mutable_op_payment()->set_amount(10000000);

    const auto shortHash = parse_hex("00010203040506070809101112131415161718192021222324252627282930");
    input.mutable_memo_hash()->set_hash(shortHash.data(), shortHash.size()); // 31 bytes — invalid

    const auto serialized = input.SerializeAsString();
    auto inputStrData = TW::Data(serialized.begin(), serialized.end());
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);

    TW::TxCompiler::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    EXPECT_EQ(preSigningOutput.error(), Common::Proto::Error_invalid_memo);
    EXPECT_TRUE(preSigningOutput.data().empty());
}

TEST(StellarCompiler, Compile_MemoHash_InvalidSize) {
    const auto coin = TWCoinTypeStellar;
    TW::Stellar::Proto::SigningInput input;
    input.set_passphrase(TWStellarPassphrase_Stellar);
    input.set_account("GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI");
    input.set_fee(1000);
    input.set_sequence(2);
    input.mutable_op_payment()->set_destination(
        "GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52");
    input.mutable_op_payment()->set_amount(10000000);

    const auto shortHash = parse_hex("00010203040506070809101112131415161718192021222324252627282930");
    input.mutable_memo_hash()->set_hash(shortHash.data(), shortHash.size()); // 31 bytes — invalid

    const auto serialized = input.SerializeAsString();
    auto inputStrData = TW::Data(serialized.begin(), serialized.end());
    const auto dummySig = parse_hex("aabb");
    const auto outputData = TransactionCompiler::compileWithSignatures(coin, inputStrData, {dummySig}, {});

    TW::Stellar::Proto::SigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
    EXPECT_EQ(output.error(), Common::Proto::Error_invalid_memo);
    EXPECT_TRUE(output.signature().empty());
}

// Expected values cross-checked with stellar-sdk (Python).
TEST(StellarCompiler, CompileWithSignatures_ChangeTrust_RemoveTrustline) {
    const auto coin = TWCoinTypeStellar;
    const auto privateKey = PrivateKey(parse_hex("3c0635f8638605aed6e461cf3fa2d508dd895df1a1655ff92c79bfbeaf88d4b9"));

    TW::Stellar::Proto::SigningInput input;
    input.set_passphrase(TWStellarPassphrase_Stellar);
    input.set_account("GDFEKJIFKUZP26SESUHZONAUJZMBSODVN2XBYN4KAGNHB7LX2OIXLPUL");
    input.set_fee(10000);
    input.set_sequence(144098454883270659);
    input.mutable_op_change_trust()->mutable_asset()->set_issuer("GA6HCMBLTZS5VYYBCATRBRZ3BZJMAFUDKYYF6AH6MVCMGWMRDNSWJPIH");
    input.mutable_op_change_trust()->mutable_asset()->set_alphanum4("MOBI");
    input.mutable_op_change_trust()->set_valid_before(1613336576);
    input.mutable_op_change_trust()->set_limit(0);

    const auto serialized = input.SerializeAsString();
    const auto inputStrData = TW::Data(serialized.begin(), serialized.end());

    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);
    TW::TxCompiler::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
    EXPECT_EQ(hex(preSigningOutput.data_hash()), "1881cb107002da73fccf7db634054143507d82e4102658b47dd60a2948ffe17c");

    const auto signature = parse_hex("5ae0b895ad63da6166b82e465e744c2243668eeed0ec9db978bc39f5e44d47c2"
                                     "71d7c9f028d8a972eae1b63855eee071387518e358ddc58a0add5e1f1d7fa707");
    const auto* const expectedTx = "AAAAAMpFJQVVMv16RJUPlzQUTlgZOHVurhw3igGacP1305F1AAAnEAH/8MgAAAADAAAAAQAAAAAAAAAAAAAAAGApkAAAAAAAAAAAAQAAAAAAAAAGAAAAAU1PQkkAAAAAPHEwK55l2uMBECcQxzsOUsAWg1YwXwD+ZUTDWZEbZWQAAAAAAAAAAAAAAAAAAAABd9ORdQAAAEBa4LiVrWPaYWa4LkZedEwiQ2aO7tDsnbl4vDn15E1HwnHXyfAo2Kly6uG2OFXu4HE4dRjjWN3FigrdXh8df6cH";

    const auto outputData = TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {});
    {
        TW::Stellar::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.error(), Common::Proto::OK);
        EXPECT_EQ(output.signature(), expectedTx);
    }
    { // Same result when signing with the private key.
        auto signingInput = input;
        signingInput.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

        TW::Stellar::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);
        EXPECT_EQ(output.signature(), expectedTx);
    }
}

TEST(StellarCompiler, PreImageHashes_ChangeTrust_NegativeLimit) {
    const auto coin = TWCoinTypeStellar;
    TW::Stellar::Proto::SigningInput input;
    input.set_passphrase(TWStellarPassphrase_Stellar);
    input.set_account("GDFEKJIFKUZP26SESUHZONAUJZMBSODVN2XBYN4KAGNHB7LX2OIXLPUL");
    input.set_fee(10000);
    input.set_sequence(144098454883270659);
    input.mutable_op_change_trust()->mutable_asset()->set_issuer("GA6HCMBLTZS5VYYBCATRBRZ3BZJMAFUDKYYF6AH6MVCMGWMRDNSWJPIH");
    input.mutable_op_change_trust()->mutable_asset()->set_alphanum4("MOBI");
    input.mutable_op_change_trust()->set_limit(-1);

    const auto serialized = input.SerializeAsString();
    const auto inputStrData = TW::Data(serialized.begin(), serialized.end());
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);

    TW::TxCompiler::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    EXPECT_EQ(preSigningOutput.error(), Common::Proto::Error_invalid_params);
    EXPECT_TRUE(preSigningOutput.data().empty());
    EXPECT_TRUE(preSigningOutput.data_hash().empty());
}

TEST(StellarCompiler, Compile_ChangeTrust_NegativeLimit) {
    const auto coin = TWCoinTypeStellar;
    TW::Stellar::Proto::SigningInput input;
    input.set_passphrase(TWStellarPassphrase_Stellar);
    input.set_account("GDFEKJIFKUZP26SESUHZONAUJZMBSODVN2XBYN4KAGNHB7LX2OIXLPUL");
    input.set_fee(10000);
    input.set_sequence(144098454883270659);
    input.mutable_op_change_trust()->mutable_asset()->set_issuer("GA6HCMBLTZS5VYYBCATRBRZ3BZJMAFUDKYYF6AH6MVCMGWMRDNSWJPIH");
    input.mutable_op_change_trust()->mutable_asset()->set_alphanum4("MOBI");
    input.mutable_op_change_trust()->set_limit(-1);

    const auto serialized = input.SerializeAsString();
    const auto inputStrData = TW::Data(serialized.begin(), serialized.end());
    const auto dummySig = parse_hex("aabb");
    const auto outputData = TransactionCompiler::compileWithSignatures(coin, inputStrData, {dummySig}, {});

    TW::Stellar::Proto::SigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
    EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    EXPECT_TRUE(output.signature().empty());
}
