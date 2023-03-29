// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "uint256.h"
#include "HexCoding.h"

#include <vector>
#include <tuple>

#include <gtest/gtest.h>

namespace TW {

// Test data: uint256_t, hex binary representation, string representation
const std::vector<std::tuple<uint256_t, const char*, const char*>> testData = {
    {0, "00", "0"},
    {1, "01", "1"},
    {7, "07", "7"},
    {100, "64", "100"},
    {255, "ff", "255"},
    {256, "0100", "256"},
    {65535, "ffff", "65535"},
    {1'000'000, "0f4240", "1000000"},
    {
        load(parse_hex("123456789abcdef0")),
        "123456789abcdef0", 
        "1311768467463790320"
    },
    {
        load(parse_hex("123456789abcdef123456789abcdef")),
        "123456789abcdef123456789abcdef",
        "94522879700260683142460330790866415"
    },
    {
        load(parse_hex("1000000000000000000000000000000000000000")),
        "1000000000000000000000000000000000000000",
        "91343852333181432387730302044767688728495783936"
    },
};

TEST(Uint256, storeLoadBasic) {
    EXPECT_EQ(hex(store(uint256_t(3))), "03");
    EXPECT_EQ(load(parse_hex("03")), uint256_t(3));
}

TEST(Uint256, storeLoadStore) {
    for(const auto& testi: testData) {
        const uint256_t dataI = std::get<0>(testi);
        const char* dataD = std::get<1>(testi);

        const uint256_t i = dataI;

        const Data d = store(i);
        EXPECT_EQ(hex(d), dataD);

        const uint256_t i2 = load(d);
        EXPECT_EQ(i2, dataI);

        const Data d2 = store(i2);
        EXPECT_EQ(hex(d2), dataD);
    }
}

TEST(Uint256, toString) {
    for(const auto& testi: testData) {
        const uint256_t dataI = std::get<0>(testi);
        const char* dataS = std::get<2>(testi);

        EXPECT_EQ(toString(dataI), dataS);
    }
}

TEST(Uint256, storePadding) {
    EXPECT_EQ(hex(store(uint256_t(3))), "03");
    EXPECT_EQ(hex(store(uint256_t(1'000'000))), "0f4240");

    EXPECT_EQ(hex(store(uint256_t(3), 4)), "00000003");
    EXPECT_EQ(hex(store(uint256_t(3), 32)), "0000000000000000000000000000000000000000000000000000000000000003");
    EXPECT_EQ(hex(store(uint256_t(3), 64)), "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003");

    EXPECT_EQ(hex(store(uint256_t(1'000'000), 32)), "00000000000000000000000000000000000000000000000000000000000f4240");
    EXPECT_EQ(hex(store(uint256_t(1'000'000), 2)), "0f4240");
}

TEST(Uint256, loadWithLeadingZero) {
    EXPECT_EQ(load(parse_hex("0f4240")), uint256_t(1'000'000));
    EXPECT_EQ(load(parse_hex("000f4240")), uint256_t(1'000'000));
    EXPECT_EQ(load(parse_hex("000000000f4240")), uint256_t(1'000'000));
    EXPECT_EQ(load(parse_hex("0000000000000000000000000000000000000000000000000000000000000003")), uint256_t(3));
    EXPECT_EQ(load(parse_hex("00000000000000000000000000000000000000000000000000000000000f4240")), uint256_t(1'000'000));
}

TEST(Uint256, LoadEmpty) {
    EXPECT_EQ(load(parse_hex("")), uint256_t(0));
    EXPECT_EQ(load(parse_hex("00")), uint256_t(0));
    EXPECT_EQ(load(parse_hex("0000")), uint256_t(0));
}

TEST(Uint256, LoadWithOffset) {
    EXPECT_EQ(loadWithOffset(parse_hex("0000000000000000000000000000000000000000000000000000000000000003"), 0), uint256_t(3));
    EXPECT_EQ(loadWithOffset(parse_hex("abcdef0000000000000000000000000000000000000000000000000000000000000003"), 3), uint256_t(3));
    EXPECT_EQ(loadWithOffset(parse_hex("0000000000000000000000000000000000000000000000000000000000000003"), 1), uint256_t(0)); // not enough bytes
}

TEST(Uint256, loadStringProtobuf) {
    const Data data = parse_hex("03");
    const std::string str = std::string(reinterpret_cast<const char*>(data.data()), data.size());
    EXPECT_EQ(load(str), uint256_t(3));
}

} // namespace
