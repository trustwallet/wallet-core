// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TestCases.h"
#include "TestUtilities.h"
#include "TransactionCompiler.h"
#include "proto/Pactus.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <gtest/gtest.h>

using namespace TW;

TEST(PactusCompiler, CompileAndSign) {
    for (const auto& testCase : TEST_CASES) {
        auto input = testCase.createSigningInput();
        auto inputString = input.SerializeAsString();
        auto inputStrData = TW::Data(inputString.begin(), inputString.end());

        // Pre-hash the transaction.
        auto preImageHashesData = TransactionCompiler::preImageHashes(TWCoinTypePactus, inputStrData);
        TxCompiler::Proto::PreSigningOutput preSigningOutput;
        preSigningOutput.ParseFromArray(preImageHashesData.data(), static_cast<int>(preImageHashesData.size()));
        auto actualDataToSign = data(preSigningOutput.data());

        EXPECT_EQ(preSigningOutput.error(), Common::Proto::OK);
        EXPECT_EQ(hex(actualDataToSign), testCase.dataToSign);

        // Sign the pre-hash data.
        auto privateKey = PrivateKey(parse_hex(PRIVATE_KEY_HEX));
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519).bytes;
        auto signature = privateKey.sign(actualDataToSign, TWCurveED25519);
        EXPECT_EQ(hex(signature), testCase.signature);

        // Compile the transaction.
        auto outputData = TransactionCompiler::compileWithSignatures(TWCoinTypePactus, inputStrData, {signature}, {publicKey});
        TW::Pactus::Proto::SigningOutput output;
        output.ParseFromArray(outputData.data(), static_cast<int>(outputData.size()));

        EXPECT_EQ(output.error(), Common::Proto::OK);
        ASSERT_EQ(hex(output.signed_transaction_data()), testCase.signedData);
        ASSERT_EQ(hex(output.signature()), testCase.signature);
        ASSERT_EQ(hex(output.transaction_id()), testCase.transactionID);
    }
}
