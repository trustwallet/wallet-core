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

#include "proto/Ripple.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Ripple::tests {

TEST(RippleCompiler, CompileWithSignatures) {
    const auto coin = TWCoinTypeXRP;
    /// Step 1: Prepare transaction input (protobuf)
    auto key = parse_hex("ba005cd605d8a02e3d5dfd04234cef3a3ee4f76bfbad2722d1fb5af8e12e6764");
    auto input = TW::Ripple::Proto::SigningInput();
    auto privateKey = TW::PrivateKey(key);
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    input.set_amount(29000000);
    input.set_fee(200000);
    input.set_sequence(1);
    input.set_account("rDpysuumkweqeC7XdNgYNtzL5GxbdsmrtF");
    input.set_destination("rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF");
    input.set_public_key(publicKey.bytes.data(), publicKey.bytes.size());
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    auto preImage = preSigningOutput.data();
    EXPECT_EQ(hex(preImage),
              "5354580012000022800000002400000001614000000001ba8140684000000000030d407321026cc34b92"
              "cefb3a4537b3edb0b6044c04af27c01583c577823ecc69a9a21119b681148400b6b6d08d5d495653d73e"
              "da6804c249a5148883148132e4e20aecf29090ac428a9c43f230a829220d");
    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash),
              "8624dbbd5da9ccc8f7a50faf8af8709837db72f51a50cac15a6cd28ce6107b3d");
    // Simulate signature, normally obtained from signature server
    const auto signature = privateKey.sign(parse_hex("8624dbbd5da9ccc8f7a50faf8af8709837db72f51a50cac15a6cd28ce6107b3d"), TWCurveSECP256k1);
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKey.bytes});
    const auto ExpectedTx = std::string(
        "12000022800000002400000001614000000001ba8140684000000000030d407321026cc34b92cefb3a45"
        "37b3edb0b6044c04af27c01583c577823ecc69a9a21119b6744630440220067f20b3eebfc7107dd0bcc7"
        "2337a236ac3be042c0469f2341d76694a17d4bb9022048393d7ee7dcb729783b33f5038939ddce1bb833"
        "7e66d752974626854556bbb681148400b6b6d08d5d495653d73eda6804c249a5148883148132e4e20aec"
        "f29090ac428a9c43f230a829220d");
    EXPECT_EQ(outputData.size(), 185ul);

    {
        TW::Ripple::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 182ul);
        ASSERT_EQ(output.error(), TW::Common::Proto::SigningError::OK);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::Ripple::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        signingInput.set_private_key(key.data(), key.size());

        TW::Ripple::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Negative: not enough signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {publicKey.bytes});
        Ripple::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {}, {});
        Ripple::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

} // namespace TW::Ripple::tests