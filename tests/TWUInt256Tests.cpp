// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWUInt256.h>

#include <gtest/gtest.h>

TEST(TWUInt256Tests, ZeroIsZero) {
    auto zero = TWUInt256Zero();
    ASSERT_TRUE(TWUInt256IsZero(zero));
}

TEST(TWUInt256Tests, OneIsNotZero) {
    auto one = TWUInt256One();
    ASSERT_FALSE(TWUInt256IsZero(one));
}

TEST(TWUInt256Tests, CreateWithData) {
    uint8_t bytes[] = {0x1b, 0xc1, 0x6d, 0x67, 0x4e, 0xc8, 0x00, 0x00};
    auto data = WRAPD(TWDataCreateWithBytes(bytes, 8));
    auto number = TWUInt256();
    TWUInt256InitWithData(&number, data.get());

    auto actual = WRAPD(TWUInt256Data(number));

    ASSERT_EQ(TWDataSize(actual.get()), 32);
    for (auto i = 0; i < 24; i += 1)
        ASSERT_EQ(TWDataBytes(actual.get())[i], 0);
    ASSERT_EQ(TWDataBytes(actual.get())[24], 0x1b);
    ASSERT_EQ(TWDataBytes(actual.get())[25], 0xc1);
    ASSERT_EQ(TWDataBytes(actual.get())[26], 0x6d);
    ASSERT_EQ(TWDataBytes(actual.get())[27], 0x67);
    ASSERT_EQ(TWDataBytes(actual.get())[28], 0x4e);
    ASSERT_EQ(TWDataBytes(actual.get())[29], 0xc8);
    ASSERT_EQ(TWDataBytes(actual.get())[30], 0x00);
    ASSERT_EQ(TWDataBytes(actual.get())[31], 0x00);
}

TEST(TWUInt256Tests, CreateWithString) {
    auto number = TWUInt256();
    TWUInt256InitWithString(&number, STRING("12345678901234567890").get());
    auto string = WRAPS(TWUInt256Description(number));
    assertStringsEqual(string, "12345678901234567890");
}

TEST(TWUInt256Tests, FormatZeroOne) {
    auto zero = TWUInt256Zero();
    auto one = TWUInt256One();
    ASSERT_STREQ(TWStringUTF8Bytes(TWUInt256Format(zero, 0)), "0");
    ASSERT_STREQ(TWStringUTF8Bytes(TWUInt256Format(one, 0)), "1");
    ASSERT_STREQ(TWStringUTF8Bytes(TWUInt256Format(zero, 1)), "0.0");
    ASSERT_STREQ(TWStringUTF8Bytes(TWUInt256Format(one, 1)), "0.1");
    ASSERT_STREQ(TWStringUTF8Bytes(TWUInt256Format(zero, 2)), "0.0");
    ASSERT_STREQ(TWStringUTF8Bytes(TWUInt256Format(one, 2)), "0.01");
}

TEST(TWUInt256Tests, FormatEndingInZero) {
    auto x = TWUInt256();
    TWUInt256InitWithUInt64(&x, 1234567890);
    ASSERT_STREQ(TWStringUTF8Bytes(TWUInt256Format(x, 0)), "1234567890");
    ASSERT_STREQ(TWStringUTF8Bytes(TWUInt256Format(x, 1)), "123456789.0");
    ASSERT_STREQ(TWStringUTF8Bytes(TWUInt256Format(x, 5)), "12345.6789");
    ASSERT_STREQ(TWStringUTF8Bytes(TWUInt256Description(x)), "1234567890");
}

TEST(TWUInt256Tests, FormatPad) {
    auto x = TWUInt256();
    TWUInt256InitWithUInt64(&x, 1234);
    ASSERT_STREQ(TWStringUTF8Bytes(TWUInt256Format(x, 1)), "123.4");
    ASSERT_STREQ(TWStringUTF8Bytes(TWUInt256Format(x, 2)), "12.34");
    ASSERT_STREQ(TWStringUTF8Bytes(TWUInt256Format(x, 3)), "1.234");
    ASSERT_STREQ(TWStringUTF8Bytes(TWUInt256Format(x, 4)), "0.1234");
    ASSERT_STREQ(TWStringUTF8Bytes(TWUInt256Format(x, 5)), "0.01234");
}
