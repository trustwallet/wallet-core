// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWData.h>
#include "TestUtilities.h"

#include <gtest/gtest.h>

TEST(TWData, CreateWithHexString) {
    {
        const auto data = WRAPD(TWDataCreateWithHexString(STRING("deadbeef").get()));
        ASSERT_EQ(TWDataSize(data.get()), 4ul);
        EXPECT_EQ(TWDataBytes(data.get())[0], 0xde);
        EXPECT_EQ(TWDataBytes(data.get())[1], 0xad);
        EXPECT_EQ(TWDataBytes(data.get())[2], 0xbe);
        EXPECT_EQ(TWDataBytes(data.get())[3], 0xef);
        assertHexEqual(data, "deadbeef");
    }

    {
        const auto data = WRAPD(TWDataCreateWithHexString(STRING("00").get()));
        ASSERT_EQ(TWDataSize(data.get()), 1ul);
        EXPECT_EQ(TWDataBytes(data.get())[0], 0);
        assertHexEqual(data, "00");
    }

    {
        // with 0x prefix
        const auto data = WRAPD(TWDataCreateWithHexString(STRING("0xdeadbeef").get()));
        assertHexEqual(data, "deadbeef");
    }

    {
        // uppercase
        const auto data = WRAPD(TWDataCreateWithHexString(STRING("DEADBEEF").get()));
        assertHexEqual(data, "deadbeef");
    }

    {
        // odd length is invalid (intended grouping to bytes is not obvious)
        const auto data = WRAPD(TWDataCreateWithHexString(STRING("012").get()));
        assertHexEqual(data, "");
    }

    {
        // null input
        TWString* nullstring = nullptr;
        const auto data = WRAPD(TWDataCreateWithHexString(nullstring));
        ASSERT_EQ(data.get(), nullptr);
    }
}

TEST(TWData, CreateWithBytes) {
    const uint8_t bytes[] = {0xde, 0xad, 0xbe, 0xef};
    const auto data = WRAPD(TWDataCreateWithBytes(bytes, 4));
    assertHexEqual(data, "deadbeef");
}

TEST(TWData, CreateWithSize) {
    std::size_t n = 12;
    const auto data = WRAPD(TWDataCreateWithSize(n));
    ASSERT_EQ(TWDataSize(data.get()), n);
    for (auto i = 0ul; i < n; ++i) {
        EXPECT_EQ(TWDataBytes(data.get())[i], 0);
    }
}

TEST(TWData, CreateWithData) {
    const auto data1 = WRAPD(TWDataCreateWithHexString(STRING("deadbeef").get()));
    const auto data2 = WRAPD(TWDataCreateWithData(data1.get()));
    assertHexEqual(data2, "deadbeef");
}

TEST(TWData, Delete) {
    const auto data = TWDataCreateWithSize(8);
    ASSERT_TRUE(data != nullptr);
    TWDataDelete(data);
}

TEST(TWData, Get) {
    const auto data = WRAPD(TWDataCreateWithHexString(STRING("deadbeef").get()));
    assertHexEqual(data, "deadbeef");
    EXPECT_EQ(TWDataGet(data.get(), 0), 0xde);
    EXPECT_EQ(TWDataGet(data.get(), 1), 0xad);
    EXPECT_EQ(TWDataGet(data.get(), 2), 0xbe);
    EXPECT_EQ(TWDataGet(data.get(), 3), 0xef);
}

TEST(TWData, Set) {
    const auto data = WRAPD(TWDataCreateWithHexString(STRING("deadbeef").get()));
    assertHexEqual(data, "deadbeef");
    TWDataSet(data.get(), 1, 0xff);
    assertHexEqual(data, "deffbeef");
}

TEST(TWData, CopyBytes) {
    const auto data = WRAPD(TWDataCreateWithHexString(STRING("deadbeef").get()));
    assertHexEqual(data, "deadbeef");
    uint8_t buffer[4];
    TWDataCopyBytes(data.get(), 0, 4, buffer);
    const auto data2 = WRAPD(TWDataCreateWithBytes(buffer, 4));
    assertHexEqual(data2, "deadbeef");
}

TEST(TWData, ReplaceBytes) {
    const auto data = WRAPD(TWDataCreateWithHexString(STRING("deadbeef").get()));
    assertHexEqual(data, "deadbeef");
    const uint8_t bytes[] = {0x12, 0x34};
    TWDataReplaceBytes(data.get(), 1, 2, bytes);
    assertHexEqual(data, "de1234ef");
}

TEST(TWData, Append) {
    {
        const auto data = WRAPD(TWDataCreateWithHexString(STRING("deadbeef").get()));
        assertHexEqual(data, "deadbeef");
        TWDataAppendByte(data.get(), 0x12);
        assertHexEqual(data, "deadbeef12");
    }

    {
        const auto data = WRAPD(TWDataCreateWithHexString(STRING("deadbeef").get()));
        assertHexEqual(data, "deadbeef");
        const uint8_t bytes[] = {0x12, 0x34};
        TWDataAppendBytes(data.get(), bytes, 2);
        assertHexEqual(data, "deadbeef1234");
    }

    {
        const auto data = WRAPD(TWDataCreateWithHexString(STRING("deadbeef").get()));
        assertHexEqual(data, "deadbeef");
        const auto data2 = WRAPD(TWDataCreateWithHexString(STRING("1234").get()));
        assertHexEqual(data2, "1234");
        TWDataAppendData(data.get(), data2.get());
        assertHexEqual(data, "deadbeef1234");
    }
}

TEST(TWData, Reverse) {
    const auto data = WRAPD(TWDataCreateWithHexString(STRING("deadbeef").get()));
    assertHexEqual(data, "deadbeef");
    TWDataReverse(data.get());
    assertHexEqual(data, "efbeadde");
}

TEST(TWData, Reset) {
    const auto data = WRAPD(TWDataCreateWithHexString(STRING("deadbeef").get()));
    assertHexEqual(data, "deadbeef");
    TWDataReset(data.get());
    assertHexEqual(data, "00000000");
}

TEST(TWData, Equal) {
    const auto data1 = WRAPD(TWDataCreateWithHexString(STRING("deadbeef").get()));
    const auto data2 = WRAPD(TWDataCreateWithHexString(STRING("deadbeef").get()));
    ASSERT_TRUE(TWDataEqual(data1.get(), data2.get()));
}
