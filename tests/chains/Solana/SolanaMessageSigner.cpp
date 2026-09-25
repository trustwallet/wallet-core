// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWMessageSigner.h>
#include <TrustWalletCore/TWString.h>

#include "Data.h"
#include "HexCoding.h"
#include "proto/Solana.pb.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>
#include <string>

namespace TW::Solana {

TEST(SolanaMessageSigner, Sign) {
    const auto privateKey = parse_hex("44f480ca27711895586074a14c552e58cc52e66a58edb6c58cf9b9b7295d4a2d");
    const auto message = "Hello world";

    Proto::MessageSigningInput input;
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_message(message);

    const auto inputData = data(input.SerializeAsString());
    const auto inputDataPtr = WRAPD(TWDataCreateWithBytes(inputData.data(), inputData.size()));

    Proto::MessageSigningOutput output;
    const auto outputDataPtr = WRAPD(TWMessageSignerSign(TWCoinTypeSolana, inputDataPtr.get()));
    output.ParseFromArray(TWDataBytes(outputDataPtr.get()), static_cast<int>(TWDataSize(outputDataPtr.get())));

    EXPECT_EQ(output.error(), Common::Proto::SigningError::OK);
    EXPECT_EQ(output.signature(), "2iBZ6zrQRKHcbD8NWmm552gU5vGvh1dk3XV4jxnyEdRKm8up8AeQk1GFr9pJokSmchw7i9gMtNyFBdDt8tBxM1cG");
}

TEST(SolanaMessageSigner, Verify) {
    const auto publicKey = parse_hex("ee6d61a89fc8f9909585a996bb0d2b2ac69ae23b5acf39a19f32631239ba06f9");
    const auto message = "Hello world";
    const auto signature = "2iBZ6zrQRKHcbD8NWmm552gU5vGvh1dk3XV4jxnyEdRKm8up8AeQk1GFr9pJokSmchw7i9gMtNyFBdDt8tBxM1cG";

    Proto::MessageVerifyingInput input;
    input.set_public_key(publicKey.data(), publicKey.size());
    input.set_message(message);
    input.set_signature(signature);

    const auto inputData = data(input.SerializeAsString());
    const auto inputDataPtr = WRAPD(TWDataCreateWithBytes(inputData.data(), inputData.size()));

    EXPECT_TRUE(TWMessageSignerVerify(TWCoinTypeSolana, inputDataPtr.get()));
}

} // namespace TW::Solana
