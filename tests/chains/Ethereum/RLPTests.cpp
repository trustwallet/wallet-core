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
