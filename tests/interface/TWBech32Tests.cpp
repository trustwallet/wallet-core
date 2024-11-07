// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/TWBech32.h>

#include <gtest/gtest.h>

TEST(TWBech32, Encode) {
    const auto hrp = STRING("abcdef");
    const auto data = DATA("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");
    const auto result = WRAPS(TWBech32Encode(hrp.get(), data.get()));
    assertStringsEqual(result, "abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw");
}

TEST(TWBech32, Decode) {
    const auto input = STRING("abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw");
    const auto result = WRAPD(TWBech32Decode(input.get()));
    assertHexEqual(result, "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");
}

TEST(TWBech32, Decode_WrongChecksumVariant) {
    const auto input = STRING("abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx");  // This is a Bech32m variant, not Bech32 variant. So it should fail.
    const auto result = WRAPD(TWBech32Decode(input.get()));
    ASSERT_EQ(result.get(), nullptr);
}
