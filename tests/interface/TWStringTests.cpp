// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <gtest/gtest.h>

TEST(StringTests, HexZero) {
    uint8_t bytes[] = { 0 };
    auto data = WRAPD(TWDataCreateWithBytes(bytes, 1));
    auto zero = WRAPS(TWStringCreateWithHexData(data.get()));
    ASSERT_STREQ(TWStringUTF8Bytes(zero.get()), "00");
}

TEST(StringTests, HexNumber) {
    uint8_t bytes[] = { 0xde, 0xad, 0xbe, 0xef };
    auto data = WRAPD(TWDataCreateWithBytes(bytes, 4));
    auto string = WRAPS(TWStringCreateWithHexData(data.get()));
    ASSERT_STREQ(TWStringUTF8Bytes(string.get()), "deadbeef");
}
