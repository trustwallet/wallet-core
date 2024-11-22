// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/TWBech32.h>

#include <gtest/gtest.h>

TEST(TWBech32, Encode) {
    const auto hrp = STRING("abcdef");
    const auto data = DATA("00443214c74254b635cf84653a56d7c675be77df");
    const auto result = WRAPS(TWBech32Encode(hrp.get(), data.get()));
    assertStringsEqual(result, "abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw");
}

TEST(TWBech32, Decode) {
    const auto input = STRING("abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw");
    const auto result = WRAPD(TWBech32Decode(input.get()));
    assertHexEqual(result, "00443214c74254b635cf84653a56d7c675be77df");
}

TEST(TWBech32, Decode_WrongChecksumVariant) {
    // This is a Bech32m variant, not Bech32 variant. So it should fail using Bech32 decoder.
    const auto input = STRING("abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx");
    const auto result = WRAPD(TWBech32Decode(input.get()));
    ASSERT_EQ(result.get(), nullptr);
}

TEST(TWBech32, EncodeM) {
    const auto hrp = STRING("abcdef");
    const auto data = DATA("ffbbcdeb38bdab49ca307b9ac5a928398a418820");
    const auto result = WRAPS(TWBech32EncodeM(hrp.get(), data.get()));
    assertStringsEqual(result, "abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx");
}

TEST(TWBech32, DecodeM) {
    const auto input = STRING("abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx");
    auto result = WRAPD(TWBech32DecodeM(input.get()));
    assertHexEqual(result, "ffbbcdeb38bdab49ca307b9ac5a928398a418820");
}

TEST(TWBech32, DecodeM_WrongChecksumVariant) {
    // This is a Bech32 variant, not Bech32m variant. So it should fail using Bech32M decoder.
    const auto input = STRING("abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw");
    const auto result = WRAPD(TWBech32DecodeM(input.get()));
    ASSERT_EQ(result.get(), nullptr);
}
