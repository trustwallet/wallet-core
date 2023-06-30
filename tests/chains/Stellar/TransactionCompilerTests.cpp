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
