// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ethereum/RLP.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Ethereum;
using boost::multiprecision::uint256_t;

TEST(RLP, Strings) {
    EXPECT_EQ(hex(RLP::encode("")), "80");
    EXPECT_EQ(hex(RLP::encode("dog")), "83646f67");
}

TEST(RLP, Integers) {
    EXPECT_EQ(hex(RLP::encode(0)), "80");
    EXPECT_EQ(hex(RLP::encode(127)), "7f");
    EXPECT_EQ(hex(RLP::encode(128)), "8180");
    EXPECT_EQ(hex(RLP::encode(256)), "820100");
    EXPECT_EQ(hex(RLP::encode(1024)), "820400");
    EXPECT_EQ(hex(RLP::encode(0xffffff)), "83ffffff");
    EXPECT_EQ(hex(RLP::encode(static_cast<uint64_t>(0xffffffffULL))), "84ffffffff");
    EXPECT_EQ(hex(RLP::encode(static_cast<uint64_t>(0xffffffffffffffULL))), "87ffffffffffffff");
}

TEST(RLP, uint256_t) {
    EXPECT_EQ(hex(RLP::encode(uint256_t(0))), "80");
    EXPECT_EQ(hex(RLP::encode(uint256_t(1))), "01");
    EXPECT_EQ(hex(RLP::encode(uint256_t(127))), "7f");
    EXPECT_EQ(hex(RLP::encode(uint256_t(128))), "8180");
    EXPECT_EQ(hex(RLP::encode(uint256_t(256))), "820100");
    EXPECT_EQ(hex(RLP::encode(uint256_t(1024))), "820400");
    EXPECT_EQ(hex(RLP::encode(uint256_t(0xffffff))), "83ffffff");
    EXPECT_EQ(hex(RLP::encode(uint256_t(0xffffffffULL))), "84ffffffff");
    EXPECT_EQ(hex(RLP::encode(uint256_t(0xffffffffffffffULL))), "87ffffffffffffff");
    EXPECT_EQ(
        hex(RLP::encode(uint256_t("0x102030405060708090a0b0c0d0e0f2"))),
        "8f102030405060708090a0b0c0d0e0f2"
    );
    EXPECT_EQ(
        hex(RLP::encode(uint256_t("0x0100020003000400050006000700080009000a000b000c000d000e01"))),
        "9c0100020003000400050006000700080009000a000b000c000d000e01"
    );
    EXPECT_EQ(
        hex(RLP::encode(uint256_t("0x0100000000000000000000000000000000000000000000000000000000000000"))),
        "a00100000000000000000000000000000000000000000000000000000000000000"
    );
}

TEST(RLP, PutInt) {
    EXPECT_EQ(hex(RLP::putint(0)), "00");
    EXPECT_EQ(hex(RLP::putint(1)), "01");
    EXPECT_EQ(hex(RLP::putint(0x21)), "21");
    EXPECT_EQ(hex(RLP::putint(0x4321)), "4321");
    EXPECT_EQ(hex(RLP::putint(0x654321)), "654321");
    EXPECT_EQ(hex(RLP::putint(0x87654321)), "87654321");
    EXPECT_EQ(hex(RLP::putint(0xa987654321)), "a987654321");
    EXPECT_EQ(hex(RLP::putint(0xcba987654321)), "cba987654321");
    EXPECT_EQ(hex(RLP::putint(0xedcba987654321)), "edcba987654321");
    EXPECT_EQ(hex(RLP::putint(0x21edcba987654321)), "21edcba987654321");
    EXPECT_EQ(hex(RLP::putint(0xffffffffffffffff)), "ffffffffffffffff");
}

TEST(RLP, Lists) {
    EXPECT_EQ(hex(RLP::encodeList(std::vector<int>())), "c0");
    EXPECT_EQ(hex(RLP::encodeList(std::vector<int>{1, 2, 3})), "c3010203");
    EXPECT_EQ(hex(RLP::encodeList(std::vector<std::string>{"cat", "dog"})), "c88363617483646f67");
    const auto encoded = RLP::encodeList(std::vector<int>(1024));
    const auto prefix = std::string("f90400");
    ASSERT_TRUE(std::equal(prefix.begin(), prefix.end(), hex(encoded).begin()));
}

TEST(RLP, Invalid) {
    ASSERT_TRUE(RLP::encode(-1).empty());
    ASSERT_TRUE(RLP::encodeList(std::vector<int>{0, -1}).empty());
}
