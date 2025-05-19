// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWMessageSigner.h>
#include <TrustWalletCore/TWString.h>

#include "Data.h"
#include "HexCoding.h"
#include "proto/Sui.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>
#include <string>

namespace TW::Sui {

TEST(SuiMessageSigner, Sign) {
    const auto privateKey = parse_hex("44f480ca27711895586074a14c552e58cc52e66a58edb6c58cf9b9b7295d4a2d");
    const auto message = "Hello world";

    Proto::MessageSigningInput input;
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_message(message);

    const auto inputData = data(input.SerializeAsString());
    const auto inputDataPtr = WRAPD(TWDataCreateWithBytes(inputData.data(), inputData.size()));

    Proto::MessageSigningOutput output;
    const auto outputDataPtr = WRAPD(TWMessageSignerSign(TWCoinTypeSui, inputDataPtr.get()));
    output.ParseFromArray(TWDataBytes(outputDataPtr.get()), static_cast<int>(TWDataSize(outputDataPtr.get())));

    EXPECT_EQ(output.error(), Common::Proto::SigningError::OK);
    EXPECT_EQ(output.signature(), "ABUNBl59ILPhyGpdgWpXJIQtEIMidR27As1771Hn7j9wVR/5IetQslRPMBrUC2THM+yGHw7h2N/Mr/0DMOpXLQ7ubWGon8j5kJWFqZa7DSsqxpriO1rPOaGfMmMSOboG+Q==");
}

TEST(SuiMessageSigner, Verify) {
    const auto publicKey = parse_hex("ee6d61a89fc8f9909585a996bb0d2b2ac69ae23b5acf39a19f32631239ba06f9");
    const auto message = "Hello world";
    const auto signature = "ABUNBl59ILPhyGpdgWpXJIQtEIMidR27As1771Hn7j9wVR/5IetQslRPMBrUC2THM+yGHw7h2N/Mr/0DMOpXLQ7ubWGon8j5kJWFqZa7DSsqxpriO1rPOaGfMmMSOboG+Q==";

    Proto::MessageVerifyingInput input;
    input.set_public_key(publicKey.data(), publicKey.size());
    input.set_message(message);
    input.set_signature(signature);

    const auto inputData = data(input.SerializeAsString());
    const auto inputDataPtr = WRAPD(TWDataCreateWithBytes(inputData.data(), inputData.size()));

    EXPECT_TRUE(TWMessageSignerVerify(TWCoinTypeSui, inputDataPtr.get()));
}

TEST(SuiMessageSigner, PreImageHashes) {
    const auto publicKey = parse_hex("ee6d61a89fc8f9909585a996bb0d2b2ac69ae23b5acf39a19f32631239ba06f9");
    const auto message = "Hello world";

    Proto::MessageSigningInput input;
    input.set_message(message);

    const auto inputData = data(input.SerializeAsString());
    const auto inputDataPtr = WRAPD(TWDataCreateWithBytes(inputData.data(), inputData.size()));

    const auto outputDataPtr = WRAPD(TWMessageSignerPreImageHashes(TWCoinTypeSui, inputDataPtr.get()));

    TxCompiler::Proto::PreSigningOutput output;
    output.ParseFromArray(TWDataBytes(outputDataPtr.get()), static_cast<int>(TWDataSize(outputDataPtr.get())));

    EXPECT_EQ(output.error(), Common::Proto::SigningError::OK);
    ASSERT_EQ(hex(output.data()), "6b27c39ed22f5346dbce4eca17640e1d139012768746aaa42eafe103f2f9ede2");
    ASSERT_EQ(hex(output.data_hash()), "6b27c39ed22f5346dbce4eca17640e1d139012768746aaa42eafe103f2f9ede2");
}

} // namespace TW::Sui
