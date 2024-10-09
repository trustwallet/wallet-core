// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TestUtilities.h"
#include "TransactionCompiler.h"
#include "proto/Pactus.pb.h"
#include "proto/TransactionCompiler.pb.h"
#include "TestCases.h"

#include <gtest/gtest.h>

using namespace TW;

TEST(PactusCompiler, CompileAndSign) {
    // Set up a signing input.

    auto input = TransferTransaction1::createSigningInput();

    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());

    // Pre-hash the transaction.
    auto preImageHashesData = TransactionCompiler::preImageHashes(TWCoinTypePactus, inputStrData);
    TxCompiler::Proto::PreSigningOutput preSigningOutput;
    preSigningOutput.ParseFromArray(preImageHashesData.data(), static_cast<int>(preImageHashesData.size()));
    auto actualDataToSign = data(preSigningOutput.data());

    EXPECT_EQ(preSigningOutput.error(), Common::Proto::OK);
    EXPECT_EQ(hex(actualDataToSign), TransferTransaction1::DATA_TO_SIGN);

    // // Sign the pre-hash data.

    auto privateKey = PrivateKey(parse_hex(TransferTransaction1::PRIVATE_KEY_HEX));
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519).bytes;
    auto signature = privateKey.sign(actualDataToSign, TWCurveED25519);
    EXPECT_EQ(hex(signature), "50ac25c7125271489b0cd230549257c93fb8c6265f2914a988ba7b81c1bc47fff027412dd59447867911035ff69742d171060a1f132ac38b95acc6e39ec0bd09");

    // Compile the transaction.

    auto outputData = TransactionCompiler::compileWithSignatures(TWCoinTypePactus, inputStrData, {signature}, {publicKey});
    TW::Pactus::Proto::SigningOutput output;
    output.ParseFromArray(outputData.data(), static_cast<int>(outputData.size()));

    EXPECT_EQ(output.error(), Common::Proto::OK);
    ASSERT_EQ(hex(output.signed_transaction_data()), "000101020300e807047465737401037098338e0b6808119dfd4457ab806b9c2059b89b037a14ae24533816e7faaa6ed28fcdde8e55a7df21a09c0150ac25c7125271489b0cd230549257c93fb8c6265f2914a988ba7b81c1bc47fff027412dd59447867911035ff69742d171060a1f132ac38b95acc6e39ec0bd0995794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa");
    ASSERT_EQ(hex(output.signature()), "50ac25c7125271489b0cd230549257c93fb8c6265f2914a988ba7b81c1bc47fff027412dd59447867911035ff69742d171060a1f132ac38b95acc6e39ec0bd09");
    ASSERT_EQ(hex(output.transaction_id()), "34cd4656a98f7eb996e83efdc384cefbe3a9c52dca79a99245b4eacc0b0b4311");
}
