// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/TWBech32m.h>

#include <gtest/gtest.h>

TEST(TWBech32m, Encode) {
    auto hrp = STRING("abcdef");
    const auto data = DATA("1f1e1d1c1b1a191817161514131211100f0e0d0c0b0a09080706050403020100");
    auto result = WRAPS(TWBech32mEncode(hrp.get(), data.get()));
    assertStringsEqual(result, "abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx");
}

TEST(TWBech32m, Decode) {
    const auto input = STRING("abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx");
    auto result = WRAPD(TWBech32mDecode(input.get()));
    assertHexEqual(result, "1f1e1d1c1b1a191817161514131211100f0e0d0c0b0a09080706050403020100");
}

TEST(TWBech32m, Decode_WrongChecksumVariant) {
    const auto input = STRING("abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw"); // This is a Bech32 variant, not Bech32m variant. So it should fail.
    const auto result = WRAPD(TWBech32mDecode(input.get()));
    ASSERT_EQ(result.get(), nullptr);
}