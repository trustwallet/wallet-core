// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Data.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;

TEST(DataTests, fromVector) {
    const Data data = {1, 2, 3};
    EXPECT_EQ(data.size(), 3ul);
    EXPECT_EQ(data[1], 2);
    EXPECT_EQ(hex(data), "010203");
}

TEST(DataTests, fromHex) {
    const Data data = parse_hex("01020304");
    EXPECT_EQ(data.size(), 4ul);
    EXPECT_EQ(hex(data), "01020304");
}

TEST(DataTests, fromString) {
    const Data data = TW::data(std::string("ABC"));
    EXPECT_EQ(data.size(), 3ul);
    EXPECT_EQ(hex(data), "414243");
}

TEST(DataTests, fromBytes) {
    const std::vector<TW::byte> vec = {1, 2, 3};
    const Data data = TW::data(vec.data(), vec.size());
    EXPECT_EQ(data.size(), 3ul);
    EXPECT_EQ(hex(data), "010203");
}

TEST(DataTests, padLeft) {
    Data data = parse_hex("01020304");
    pad_left(data, 10);
    EXPECT_EQ(data.size(), 10ul);
    EXPECT_EQ(hex(data), "00000000000001020304");
}

TEST(DataTests, append) {
    Data data1 = parse_hex("01020304");
    const Data data2 = parse_hex("aeaf");
    append(data1, data2);
    EXPECT_EQ(data1.size(), 6ul);
    EXPECT_EQ(hex(data1), "01020304aeaf");
}

TEST(DataTests, appendByte) {
    Data data1 = parse_hex("01020304");
    append(data1, 5);
    EXPECT_EQ(data1.size(), 5ul);
    EXPECT_EQ(hex(data1), "0102030405");
}

TEST(DataTests, subData) {
    const Data data = parse_hex("0102030405060708090a");
    EXPECT_EQ(data.size(), 10ul);

    EXPECT_EQ(hex(subData(data, 2, 3)), "030405");
    EXPECT_EQ(hex(subData(data, 0, 10)), "0102030405060708090a");
    EXPECT_EQ(hex(subData(data, 3, 1)), "04");
    EXPECT_EQ(hex(subData(data, 3, 0)), "");
    EXPECT_EQ(hex(subData(data, 200, 3)), ""); // index too big
    EXPECT_EQ(hex(subData(data, 2, 300)), "030405060708090a"); // length too big
    EXPECT_EQ(hex(subData(data, 200, 300)), ""); // index & length too big

    EXPECT_EQ(hex(subData(data, 3)), "0405060708090a");
    EXPECT_EQ(hex(subData(data, 0)), "0102030405060708090a");
    EXPECT_EQ(hex(subData(data, 200)), ""); // index too big
}

TEST(DataTests, hasPrefix) {
    const Data data = parse_hex("0102030405060708090a");

    const Data prefix11 = parse_hex("010203");
    EXPECT_TRUE(has_prefix(data, prefix11));
    const Data prefix12 = parse_hex("01");
    EXPECT_TRUE(has_prefix(data, prefix12));
    const Data prefix13 = parse_hex("0102030405060708090a");
    EXPECT_TRUE(has_prefix(data, prefix13));

    const Data prefix21 = parse_hex("020304");
    EXPECT_FALSE(has_prefix(data, prefix21));
    const Data prefix22 = parse_hex("02");
    EXPECT_FALSE(has_prefix(data, prefix22));
    const Data prefix23 = parse_hex("bb");
    EXPECT_FALSE(has_prefix(data, prefix23));
}
