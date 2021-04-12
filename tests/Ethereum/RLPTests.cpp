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

TEST(RLP, Decode) {
    {
        // empty string
        auto decoded = RLP::decode(parse_hex("0x80")).decoded[0];
        ASSERT_EQ(std::string(decoded.begin(), decoded.end()), "");
    }

    {
        // short string
        auto decoded = RLP::decode(parse_hex("0x83636174")).decoded[0];
        ASSERT_EQ(std::string(decoded.begin(), decoded.end()), "cat");
    }

    {
        // long string
        auto encoded = parse_hex("0xb87674686973206973206120612076657279206c6f6e6720737472696e672c2074686973206973206120612076657279206c6f6e6720737472696e672c2074686973206973206120612076657279206c6f6e6720737472696e672c2074686973206973206120612076657279206c6f6e6720737472696e67");
        auto decoded = RLP::decode(encoded).decoded[0];
        ASSERT_EQ(std::string(decoded.begin(), decoded.end()), "this is a a very long string, this is a a very long string, this is a a very long string, this is a a very long string");
    }

    {
        // empty list
        auto decoded = RLP::decode(parse_hex("0xc0")).decoded;
        ASSERT_EQ(decoded.size(), 0);
    }

    {
        // short list
        auto encoded = parse_hex("0xc88363617483646f67");
        auto decoded = RLP::decode(encoded).decoded;
        ASSERT_EQ(std::string(decoded[0].begin(), decoded[0].end()), "cat");
        ASSERT_EQ(std::string(decoded[1].begin(), decoded[1].end()), "dog");
    }
}

TEST(RLP, DecodeList) {
    {
        // long list, raw ether transfer tx
        auto rawTx = parse_hex("0xf86b81a985051f4d5ce982520894515778891c99e3d2e7ae489980cb7c77b37b5e76861b48eb57e0008025a0ad01c32a7c974df9d0bd48c8d7e0ecab62e90811917aa7dc0c966751a0c3f475a00dc77d9ec68484481bdf87faac14378f4f18d477f84c0810d29480372c1bbc65");
        auto decoded = RLP::decode(rawTx);

        auto expected = std::vector<std::string>{
            "0xa9",                                                               // nonce
            "0x051f4d5ce9",                                                       // gas price
            "0x5208",                                                             // gas limit
            "0x515778891c99e3d2e7ae489980cb7c77b37b5e76",                         // to
            "0x1b48eb57e000",                                                     // amount
            "0x",                                                                 // data
            "0x25",                                                               // v
            "0xad01c32a7c974df9d0bd48c8d7e0ecab62e90811917aa7dc0c966751a0c3f475", // r
            "0x0dc77d9ec68484481bdf87faac14378f4f18d477f84c0810d29480372c1bbc65", // s
        };
        ASSERT_EQ(decoded.decoded.size(), expected.size());
        for (int i = 0; i < expected.size(); i++) {
            EXPECT_EQ(hexEncoded(decoded.decoded[i]), expected[i]);
        }
    }

    {
        // long list, raw token transfer tx
        auto rawTx = parse_hex("0xf8aa81d485077359400082db9194dac17f958d2ee523a2206206994597c13d831ec780b844a9059cbb000000000000000000000000c6b6b55c8c4971145a842cc4e5db92d879d0b3e00000000000000000000000000000000000000000000000000000000002faf0801ca02843d8ed66b9623392dc336dd36d5dd5a630b2019962869b6e50fdb4ecb5b6aca05d9ea377bc65e2921f7fc257de8135530cc74e3188b6ba57a4b9cb284393050a");
        auto decoded = RLP::decode(rawTx);

        auto expected = std::vector<std::string>{
            "0xd4",
            "0x0773594000",
            "0xdb91",
            "0xdac17f958d2ee523a2206206994597c13d831ec7",
            "0x",
            "0xa9059cbb000000000000000000000000c6b6b55c8c4971145a842cc4e5db92d879d0b3e00000000000000000000000000000000000000000000000000000000002faf080",
            "0x1c",
            "0x2843d8ed66b9623392dc336dd36d5dd5a630b2019962869b6e50fdb4ecb5b6ac",
            "0x5d9ea377bc65e2921f7fc257de8135530cc74e3188b6ba57a4b9cb284393050a",
        };
        ASSERT_EQ(decoded.decoded.size(), expected.size());
        for (int i = 0; i < expected.size(); i++) {
            EXPECT_EQ(hexEncoded(decoded.decoded[i]), expected[i]);
        }
    }
}

TEST(RLP, DecodeLength) {
    EXPECT_EQ(hex(store(RLP::decodeLength(parse_hex("00")))), "00");
    EXPECT_EQ(hex(store(RLP::decodeLength(parse_hex("01")))), "01");
    EXPECT_EQ(hex(store(RLP::decodeLength(parse_hex("fc")))), "fc");
    EXPECT_EQ(hex(store(RLP::decodeLength(parse_hex("fd0102")))), "0201");
    EXPECT_EQ(hex(store(RLP::decodeLength(parse_hex("fe01020304")))), "04030201");
    EXPECT_EQ(hex(store(RLP::decodeLength(parse_hex("ff0102030405060708")))), "0807060504030201");
    EXPECT_THROW(RLP::decodeLength(parse_hex("fd01")), std::invalid_argument);
    EXPECT_THROW(RLP::decodeLength(parse_hex("fe010203")), std::invalid_argument);
}

TEST(RLP, DecodeInvalid) {
    // decode empty data
    EXPECT_THROW(RLP::decode(Data()), std::invalid_argument);

    // incorrect length
    EXPECT_THROW(RLP::decode(parse_hex("0x81636174")), std::invalid_argument);
    EXPECT_THROW(RLP::decode(parse_hex("0xb9ffff")), std::invalid_argument);
    EXPECT_THROW(RLP::decode(parse_hex("0xc883636174")), std::invalid_argument);

    // some tests are from https://github.com/ethereum/tests/blob/develop/RLPTests/invalidRLPTest.json
    // int32 overflow
    EXPECT_THROW(RLP::decode(parse_hex("0xbf0f000000000000021111")), std::invalid_argument);

    // wrong size list
    EXPECT_THROW(RLP::decode(parse_hex("0xf80180")), std::invalid_argument);

    // bytes should be single byte
    EXPECT_THROW(RLP::decode(parse_hex("0x8100")), std::invalid_argument);

    // leading zeros in long length list
    EXPECT_THROW(RLP::decode(parse_hex("fb00000040000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f")), std::invalid_argument);
    EXPECT_THROW(RLP::decode(parse_hex("f800")), std::invalid_argument);
}
