// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include <TrustWalletCore/TWAsnParser.h>

#include <gtest/gtest.h>

TEST(TWAsnParser, EcdsaSignatureFromDer) {
    auto encoded = DATA("3046022100db421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495da022100ff715aab70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf96c71c1b1");
    auto decodedResult = WRAPD(TWAsnParserEcdsaSignatureFromDer(encoded.get()));
    assertHexEqual(decodedResult, "db421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495daff715aab70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf96c71c1b1");

    auto invalid = DATA("");
    ASSERT_EQ(TWAsnParserEcdsaSignatureFromDer(invalid.get()), nullptr);
}
