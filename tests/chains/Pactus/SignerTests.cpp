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

TEST(PactusSigner, Sign) {
    // Set up a signing input.
    auto input = TransferTransaction1::createSigningInput();

    auto privateKey = PrivateKey(parse_hex(TransferTransaction1::PRIVATE_KEY_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    TW::Pactus::Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypePactus);

    EXPECT_EQ(output.error(), Common::Proto::OK);
    ASSERT_EQ(hex(output.signed_transaction_data()), TransferTransaction1::SIGNED_TRANSACTION_DATA);
    ASSERT_EQ(hex(output.signature()), TransferTransaction1::SIGNATURE);
    ASSERT_EQ(hex(output.transaction_id()), TransferTransaction1::TRANSACTION_ID);
}
