// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ethereum/RLP.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::Ethereum::tests {

using boost::multiprecision::uint256_t;

std::string stringifyItems(const RLP::DecodedItem& di);

std::string stringifyData(const Data& data) {
    if (data.size() == 0)
        return "0";
    // try if only letters
    bool isLettersOnly = true;
    for (auto i : data) {
        if (!((i >= 'A' && i <= 'Z') || (i >= 'a' && i <= 'z') || i == ' ' || i == ',')) {
            isLettersOnly = false;
            break;
        }
    }
    if (isLettersOnly)
        return std::string("'") + std::string(data.begin(), data.end()) + "'";
    // try if it can be parsed (recursive)
    if (data.size() >= 2) {
        try {
            const auto di = RLP::decode(data);
            if (di.decoded.size() > 0 && di.remainder.size() == 0) {
                return stringifyItems(di);
            }
        } catch (...) {
        }
    }
    // any other: as hex string
    return hex(data);
}

std::string stringifyItems(const RLP::DecodedItem& di) {
    const auto n = di.decoded.size();
    if (n == 0) {
        return "-";
    }
    if (n == 1) {
        return stringifyData(di.decoded[0]);
    }
    std::string res = "(" + std::to_string(n) + ": ";
    int count = 0;
    for (auto i : di.decoded) {
        if (count++)
            res += " ";
        res += stringifyData(i);
    }
    res += ")";
    return res;
}

std::string decodeHelper(const std::string& hexData) {
    const auto data = parse_hex(hexData);
    const auto di = RLP::decode(data);
    return stringifyItems(di);
}

TEST(RLP, EncodeString) {
    EXPECT_EQ(hex(RLP::encode("")), "80");
    EXPECT_EQ(hex(RLP::encode("d")), "64");
    EXPECT_EQ(hex(RLP::encode("dog")), "83646f67");
}

TEST(RLP, EncodeInteger) {
    EXPECT_EQ(hex(RLP::encode(0)), "80");
    EXPECT_EQ(hex(RLP::encode(127)), "7f");
    EXPECT_EQ(hex(RLP::encode(128)), "8180");
    EXPECT_EQ(hex(RLP::encode(255)), "81ff");
    EXPECT_EQ(hex(RLP::encode(256)), "820100");
    EXPECT_EQ(hex(RLP::encode(1024)), "820400");
    EXPECT_EQ(hex(RLP::encode(0xffff)), "82ffff");
    EXPECT_EQ(hex(RLP::encode(0x010000)), "83010000");
    EXPECT_EQ(hex(RLP::encode(0xffffff)), "83ffffff");
    EXPECT_EQ(hex(RLP::encode(static_cast<uint64_t>(0xffffffffULL))), "84ffffffff");
    EXPECT_EQ(hex(RLP::encode(static_cast<uint64_t>(0xffffffffffffffULL))), "87ffffffffffffff");
}

TEST(RLP, EncodeUInt256) {
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
        "8f102030405060708090a0b0c0d0e0f2");
    EXPECT_EQ(
        hex(RLP::encode(uint256_t("0x0100020003000400050006000700080009000a000b000c000d000e01"))),
        "9c0100020003000400050006000700080009000a000b000c000d000e01");
    EXPECT_EQ(
        hex(RLP::encode(uint256_t("0x0100000000000000000000000000000000000000000000000000000000000000"))),
        "a00100000000000000000000000000000000000000000000000000000000000000");
}

TEST(RLP, EncodeList) {
    EXPECT_EQ(hex(RLP::encodeList(std::vector<int>())), "c0");
    EXPECT_EQ(hex(RLP::encodeList(std::vector<int>{1, 2, 3})), "c3010203");
    EXPECT_EQ(hex(RLP::encodeList(std::vector<std::string>{"a", "b"})), "c26162");
    EXPECT_EQ(hex(RLP::encodeList(std::vector<std::string>{"cat", "dog"})), "c88363617483646f67");
    {
        const auto encoded = RLP::encodeList(std::vector<int>(1024));
        EXPECT_EQ(hex(subData(encoded, 0, 20)), "f904008080808080808080808080808080808080");
    }
}

TEST(RLP, EncodeListNested) {
    const auto l11 = RLP::encodeList(std::vector<int>{1, 2, 3});
    const auto l12 = RLP::encodeList(std::vector<std::string>{"apple", "banana", "cherry"});
    const auto l21 = RLP::encodeList(std::vector<Data>{parse_hex("abcdef"), parse_hex("00010203040506070809")});
    const auto l22 = RLP::encodeList(std::vector<std::string>{"bitcoin", "beeenbee", "eth"});
    const auto l1 = RLP::encodeList(std::vector<Data>{l11, l12});
    const auto l2 = RLP::encodeList(std::vector<Data>{l21, l22});
    const auto encoded = RLP::encodeList(std::vector<Data>{l1, l2});
    EXPECT_EQ(hex(encoded), "f8479cdb84c301020395d4856170706c658662616e616e6186636865727279a9e890cf83abcdef8a0001020304050607080996d587626974636f696e88626565656e62656583657468");
}

TEST(RLP, EncodeInvalid) {
    ASSERT_TRUE(RLP::encode(-1).empty());
    ASSERT_TRUE(RLP::encodeList(std::vector<int>{0, -1}).empty());
}

TEST(RLP, DecodeInteger) {
    EXPECT_EQ(decodeHelper("00"), "00"); // not the primary encoding for 0
    EXPECT_EQ(decodeHelper("01"), "01");
    EXPECT_EQ(decodeHelper("09"), "09");
    EXPECT_EQ(decodeHelper("7f"), "7f");
    EXPECT_EQ(decodeHelper("80"), "0");
    EXPECT_EQ(decodeHelper("8180"), "80");
    EXPECT_EQ(decodeHelper("81ff"), "ff");
    EXPECT_EQ(decodeHelper("820100"), "0100");
    EXPECT_EQ(decodeHelper("820400"), "0400");
    EXPECT_EQ(decodeHelper("82ffff"), "ffff");
    EXPECT_EQ(decodeHelper("83010000"), "010000");
    EXPECT_EQ(decodeHelper("83ffffff"), "ffffff");
    EXPECT_EQ(decodeHelper("84ffffffff"), "ffffffff");
    EXPECT_EQ(decodeHelper("87ffffffffffffff"), "ffffffffffffff");
}

TEST(RLP, DecodeString) {
    EXPECT_EQ(decodeHelper("80"), "0");
    EXPECT_EQ(decodeHelper("64"), "'d'");
    EXPECT_EQ(decodeHelper("83646f67"), "'dog'");
    EXPECT_EQ(decodeHelper("83636174"), "'cat'");
    EXPECT_EQ(decodeHelper("8f102030405060708090a0b0c0d0e0f2"), "102030405060708090a0b0c0d0e0f2");
    EXPECT_EQ(decodeHelper("9c0100020003000400050006000700080009000a000b000c000d000e01"), "0100020003000400050006000700080009000a000b000c000d000e01");
    EXPECT_EQ(decodeHelper("a00100000000000000000000000000000000000000000000000000000000000000"), "0100000000000000000000000000000000000000000000000000000000000000");
    // long string
    EXPECT_EQ(decodeHelper("b87674686973206973206120612076657279206c6f6e6720737472696e672c2074686973206973206120612076657279206c6f6e6720737472696e672c2074686973206973206120612076657279206c6f6e6720737472696e672c2074686973206973206120612076657279206c6f6e6720737472696e67"),
              "'this is a a very long string, this is a a very long string, this is a a very long string, this is a a very long string'");
}

TEST(RLP, DecodeList) {
    // empty list
    EXPECT_EQ(decodeHelper("c0"), "-");
    // short list
    EXPECT_EQ(decodeHelper("c3010203"), "(3: 01 02 03)");
    EXPECT_EQ(decodeHelper("c26162"), "(2: 'a' 'b')");
    EXPECT_EQ(decodeHelper("c88363617483646f67"), "(2: 'cat' 'dog')");

    // long list, raw ether transfer tx
    EXPECT_EQ(decodeHelper("f86b81a985051f4d5ce982520894515778891c99e3d2e7ae489980cb7c77b37b5e76861b48eb57e0008025a0ad01c32a7c974df9d0bd48c8d7e0ecab62e90811917aa7dc0c966751a0c3f475a00dc77d9ec68484481bdf87faac14378f4f18d477f84c0810d29480372c1bbc65"),
              "(9: "
              "a9 "                                                               // nonce
              "051f4d5ce9 "                                                       // gas price
              "5208 "                                                             // gas limit
              "515778891c99e3d2e7ae489980cb7c77b37b5e76 "                         // to
              "1b48eb57e000 "                                                     // amount
              "0 "                                                                // data
              "25 "                                                               // v
              "ad01c32a7c974df9d0bd48c8d7e0ecab62e90811917aa7dc0c966751a0c3f475 " // r
              "0dc77d9ec68484481bdf87faac14378f4f18d477f84c0810d29480372c1bbc65"  // s
              ")");

    // long list, raw token transfer tx
    EXPECT_EQ(decodeHelper("f8aa81d485077359400082db9194dac17f958d2ee523a2206206994597c13d831ec780b844a9059cbb000000000000000000000000c6b6b55c8c4971145a842cc4e5db92d879d0b3e00000000000000000000000000000000000000000000000000000000002faf0801ca02843d8ed66b9623392dc336dd36d5dd5a630b2019962869b6e50fdb4ecb5b6aca05d9ea377bc65e2921f7fc257de8135530cc74e3188b6ba57a4b9cb284393050a"),
              "(9: "
              "d4 "
              "0773594000 "
              "db91 "
              "dac17f958d2ee523a2206206994597c13d831ec7 "
              "0 "
              "a9059cbb000000000000000000000000c6b6b55c8c4971145a842cc4e5db92d879d0b3e00000000000000000000000000000000000000000000000000000000002faf080 "
              "1c "
              "2843d8ed66b9623392dc336dd36d5dd5a630b2019962869b6e50fdb4ecb5b6ac "
              "5d9ea377bc65e2921f7fc257de8135530cc74e3188b6ba57a4b9cb284393050a"
              ")");

    {
        // long list, with 2-byte size
        const std::string elem = "0123";
        const std::size_t n = 500;
        std::vector<std::string> longarr;
        for (auto i = 0ul; i < n; ++i)
            longarr.push_back(elem);

        const Data encoded = RLP::encodeList(longarr);
        ASSERT_EQ(hex(subData(encoded, 0, 20)), "f909c48430313233843031323384303132338430");

        auto decoded = RLP::decode(encoded);
        ASSERT_EQ(decoded.decoded.size(), n);
        for (int i = 0; i < 20; i++) {
            EXPECT_EQ(hex(decoded.decoded[i]), "30313233");
        }
    }
    {
        // long list, with 3-byte size
        const std::string elem = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
        const std::size_t n = 650;
        std::vector<std::string> longarr;
        for (auto i = 0ul; i < n; ++i)
            longarr.push_back(elem);

        const Data encoded = RLP::encodeList(longarr);
        ASSERT_EQ(encoded.size(), 66304ul);
        ASSERT_EQ(hex(subData(encoded, 0, 30)), "fa0102fcb864303132333435363738393031323334353637383930313233");

        auto decoded = RLP::decode(encoded);
        ASSERT_EQ(decoded.decoded.size(), n);
    }

    // nested list
    EXPECT_EQ(decodeHelper("f8479cdb84c301020395d4856170706c658662616e616e6186636865727279a9e890cf83abcdef8a0001020304050607080996d587626974636f696e88626565656e62656583657468"),
              "(2: (2: (3: 01 02 03) (3: 'apple' 'banana' 'cherry')) (2: (2: abcdef 00010203040506070809) (3: 'bitcoin' 'beeenbee' 'eth')))");
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

TEST(RLP, putVarInt) {
    EXPECT_EQ(hex(RLP::putVarInt(0)), "00");
    EXPECT_EQ(hex(RLP::putVarInt(1)), "01");
    EXPECT_EQ(hex(RLP::putVarInt(0x21)), "21");
    EXPECT_EQ(hex(RLP::putVarInt(0xff)), "ff");
    EXPECT_EQ(hex(RLP::putVarInt(0x100)), "0100");
    EXPECT_EQ(hex(RLP::putVarInt(0x4321)), "4321");
    EXPECT_EQ(hex(RLP::putVarInt(0x654321)), "654321");
    EXPECT_EQ(hex(RLP::putVarInt(0x87654321)), "87654321");
    EXPECT_EQ(hex(RLP::putVarInt(0xa987654321)), "a987654321");
    EXPECT_EQ(hex(RLP::putVarInt(0xcba987654321)), "cba987654321");
    EXPECT_EQ(hex(RLP::putVarInt(0xedcba987654321)), "edcba987654321");
    EXPECT_EQ(hex(RLP::putVarInt(0x21edcba987654321)), "21edcba987654321");
    EXPECT_EQ(hex(RLP::putVarInt(0xffffffffffffffff)), "ffffffffffffffff");
}

TEST(RLP, parseVarInt) {
    EXPECT_EQ(hex(store(RLP::parseVarInt(1, parse_hex("00"), 0))), "00");
    EXPECT_EQ(hex(store(RLP::parseVarInt(1, parse_hex("01"), 0))), "01");
    EXPECT_EQ(hex(store(RLP::parseVarInt(1, parse_hex("fc"), 0))), "fc");
    EXPECT_EQ(hex(store(RLP::parseVarInt(1, parse_hex("ff"), 0))), "ff");
    EXPECT_EQ(hex(store(RLP::parseVarInt(1, parse_hex("abcd"), 1))), "cd");
    EXPECT_EQ(hex(store(RLP::parseVarInt(2, parse_hex("0102"), 0))), "0102");
    EXPECT_EQ(hex(store(RLP::parseVarInt(2, parse_hex("0100"), 0))), "0100");
    EXPECT_EQ(hex(store(RLP::parseVarInt(2, parse_hex("fedc"), 0))), "fedc");
    EXPECT_EQ(hex(store(RLP::parseVarInt(2, parse_hex("ffff"), 0))), "ffff");
    EXPECT_EQ(hex(store(RLP::parseVarInt(3, parse_hex("010203"), 0))), "010203");
    EXPECT_EQ(hex(store(RLP::parseVarInt(4, parse_hex("01020304"), 0))), "01020304");
    EXPECT_EQ(hex(store(RLP::parseVarInt(5, parse_hex("0102030405"), 0))), "0102030405");
    EXPECT_EQ(hex(store(RLP::parseVarInt(6, parse_hex("010203040506"), 0))), "010203040506");
    EXPECT_EQ(hex(store(RLP::parseVarInt(7, parse_hex("01020304050607"), 0))), "01020304050607");
    EXPECT_EQ(hex(store(RLP::parseVarInt(8, parse_hex("0102030405060708"), 0))), "0102030405060708");
    EXPECT_EQ(hex(store(RLP::parseVarInt(8, parse_hex("abcd0102030405060708"), 2))), "0102030405060708");
    EXPECT_THROW(RLP::parseVarInt(0, parse_hex("01"), 0), std::invalid_argument);                 // wrong size
    EXPECT_THROW(RLP::parseVarInt(9, parse_hex("010203040506070809"), 0), std::invalid_argument); // wrong size
    EXPECT_THROW(RLP::parseVarInt(4, parse_hex("0102"), 0), std::invalid_argument);               // too short
    EXPECT_THROW(RLP::parseVarInt(4, parse_hex("01020304"), 2), std::invalid_argument);           // too short
    EXPECT_THROW(RLP::parseVarInt(2, parse_hex("0002"), 0), std::invalid_argument);               // starts with 0
}

TEST(RLP, decodeLenOverflow) {
    EXPECT_THROW(RLP::decode(parse_hex("c9bffffffffffffffff7")), std::invalid_argument); // String length overflow (64 bit)
    EXPECT_THROW(RLP::decode(parse_hex("c7fbfffffffbc17f")), std::invalid_argument); // List length overflow (32 bit)
    EXPECT_THROW(RLP::decode(parse_hex("cbfffffffffffffffff7c17f")), std::invalid_argument); // List length overflow (64 bit)
}

} // namespace TW::Ethereum::tests
