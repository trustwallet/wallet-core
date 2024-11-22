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

TEST(PactusSigner, Sign) {
    for (const auto& testCase : TEST_CASES) {
        auto input = testCase.createSigningInput();

        auto privateKey = PrivateKey(parse_hex(PRIVATE_KEY_HEX));
        input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

        TW::Pactus::Proto::SigningOutput output;
        ANY_SIGN(input, TWCoinTypePactus);

        EXPECT_EQ(output.error(), Common::Proto::OK);
        ASSERT_EQ(hex(output.signed_transaction_data()), testCase.signedData);
        ASSERT_EQ(hex(output.signature()), testCase.signature);
        ASSERT_EQ(hex(output.transaction_id()), testCase.transactionID);
    }
}
