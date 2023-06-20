// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

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

TEST(StringTests, GetChar) {
    uint8_t bytes[] = { 0xde, 0xad, 0xbe, 0xef };
    auto data = WRAPD(TWDataCreateWithBytes(bytes, 4));
    auto string = WRAPS(TWStringCreateWithHexData(data.get()));
    ASSERT_STREQ(TWStringUTF8Bytes(string.get()), "deadbeef");

    ASSERT_EQ(TWStringGet(string.get(), 0), 'd');
    ASSERT_EQ(TWStringGet(string.get(), 1), 'e');
    ASSERT_EQ(TWStringGet(string.get(), 2), 'a');
    ASSERT_EQ(TWStringGet(string.get(), 3), 'd');
    ASSERT_EQ(TWStringGet(string.get(), 4), 'b');
    ASSERT_EQ(TWStringGet(string.get(), 5), 'e');
    ASSERT_EQ(TWStringGet(string.get(), 6), 'e');
    ASSERT_EQ(TWStringGet(string.get(), 7), 'f');
}