// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "Data.h"
#include "uint256.h"
#include <gtest/gtest.h>

namespace TW {

TEST(HexCoding, validation) {
    const std::string valid = "0x7d8bf18c7ce84b3e175b339c4ca93aed1dd166f1";
    const std::string invalid = "0xMQqpqMQgCBuiPkoXfgZZsJvuzCeI1zc00z6vHJj4";
    const auto bytes = parse_hex(invalid);
    const auto bytes2 = parse_hex(valid);

    ASSERT_TRUE(bytes.empty());
    ASSERT_EQ("0x" + hex(bytes2), valid);
}

TEST(HexCoding, OddLength) {
    const std::string oddHex = "0x28fa6ae00";
    const auto bytes = parse_hex(oddHex, true);
    const auto number = load(bytes);
    ASSERT_EQ(number, 11000000000);
}

TEST(HexCoding, isHexEncoded) {
    ASSERT_TRUE(is_hex_encoded("66fbe3c5c03bf5c82792f904c9f8bf28894a6aa3d213d41c20569b654aadedb3"));
    ASSERT_TRUE(is_hex_encoded("0x66fbe3c5c03bf5c82792f904c9f8bf28894a6aa3d213d41c20569b654aadedb3"));
    ASSERT_FALSE(is_hex_encoded("1x66fbe3c5c03bf5c82792f904c9f8bf28894a6aa3d213d41c20569b654aadedb3"));
    ASSERT_FALSE(is_hex_encoded("0xyahoo"));
}

TEST(HexCoding, ParseHexChecked) {
    // Valid, well-padded
    auto res = parse_hex_checked("0x7d8bf18c7ce84b3e175b339c4ca93aed1dd166f1");
    ASSERT_TRUE(res.isSuccess());
    EXPECT_EQ(hex(res.payload()), "7d8bf18c7ce84b3e175b339c4ca93aed1dd166f1");

    // Valid, well-padded, no 0x
    res = parse_hex_checked("7d8bf18c7ce84b3e175b339c4ca93aed1dd166f1");
    ASSERT_TRUE(res.isSuccess());
    EXPECT_EQ(hex(res.payload()), "7d8bf18c7ce84b3e175b339c4ca93aed1dd166f1");

    // Empty string
    res = parse_hex_checked("");
    ASSERT_TRUE(res.isSuccess());
    EXPECT_TRUE(res.payload().empty());

    // 0x string
    res = parse_hex_checked("0x");
    ASSERT_TRUE(res.isSuccess());
    EXPECT_TRUE(res.payload().empty());
}

TEST(HexCoding, ParseHexCheckedNotPaddedFailed) {
    // Odd-length, not padded, padLeft = false
    auto res = parse_hex_checked("0x123", false);
    ASSERT_FALSE(res.isSuccess());
    EXPECT_EQ(res.error(), "Invalid hex string");

    res = parse_hex_checked("123", false);
    ASSERT_FALSE(res.isSuccess());
    EXPECT_EQ(res.error(), "Invalid hex string");
}

TEST(HexCoding, ParseHexCheckedNotPaddedSucceeds) {
    // Odd-length, padLeft = true
    auto res = parse_hex_checked("0x1", true);
    ASSERT_TRUE(res.isSuccess());
    EXPECT_EQ(hex(res.payload()), "01");

    res = parse_hex_checked("abc", true);
    ASSERT_TRUE(res.isSuccess());
    EXPECT_EQ(hex(res.payload()), "0abc");
}

TEST(HexCoding, ParseHexCheckedInvalidHex) {
    // Not valid hex
    auto res = parse_hex_checked("0xMQqpqMQgCBuiPkoXfgZZsJvuzCeI1zc00z6vHJj4");
    ASSERT_FALSE(res.isSuccess());
    EXPECT_EQ(res.error(), "Invalid hex string");

    res = parse_hex_checked("nothex");
    ASSERT_FALSE(res.isSuccess());
    EXPECT_EQ(res.error(), "Invalid hex string");
}

TEST(HexCoding, PadLeftHex) {
    // Empty string
    EXPECT_EQ(internal::pad_left_hex(""), "");
    // "0x" only
    EXPECT_EQ(internal::pad_left_hex("0x"), "0x");
    // Odd-length with 0x
    EXPECT_EQ(internal::pad_left_hex("0x1", true), "0x01");
    EXPECT_EQ(internal::pad_left_hex("0x12", true), "0x12");
    EXPECT_EQ(internal::pad_left_hex("0x123", true), "0x0123");
    // Odd-length without 0x
    EXPECT_EQ(internal::pad_left_hex("1", true), "01");
    EXPECT_EQ(internal::pad_left_hex("12", true), "12");
    EXPECT_EQ(internal::pad_left_hex("123", true), "0123");
    // padLeft = false (should not pad)
    EXPECT_EQ(internal::pad_left_hex("0x1", false), "0x1");
    EXPECT_EQ(internal::pad_left_hex("123", false), "123");
}

}
