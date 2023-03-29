// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ethereum/ABI/ValueDecoder.h"
#include <HexCoding.h>

#include <gtest/gtest.h>

namespace TW::Ethereum::tests {

uint256_t decodeFromHex(std::string s) {
    auto data = parse_hex(s);
    return ABI::ValueDecoder::decodeUInt256(data);
}

TEST(EthereumAbiValueDecoder, decodeUInt256) {
    EXPECT_EQ(uint256_t(0), decodeFromHex(""));
    EXPECT_EQ(uint256_t(0), decodeFromHex("0000000000000000000000000000000000000000000000000000000000000000"));
    EXPECT_EQ(uint256_t(1), decodeFromHex("0000000000000000000000000000000000000000000000000000000000000001"));
    EXPECT_EQ(uint256_t(123456), decodeFromHex("01e240"));
    EXPECT_EQ(uint256_t(10020405371567), decodeFromHex("0000000000000000000000000000000000000000000000000000091d0eb3e2af"));
    EXPECT_EQ(uint256_t(10020405371567), decodeFromHex("0000000000000000000000000000000000000000000000000000091d0eb3e2af00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"));
}

TEST(EthereumAbiValueDecoder, decodeValue) {
    EXPECT_EQ("42", ABI::ValueDecoder::decodeValue(parse_hex("000000000000000000000000000000000000000000000000000000000000002a"), "uint"));
    EXPECT_EQ("24", ABI::ValueDecoder::decodeValue(parse_hex("0000000000000000000000000000000000000000000000000000000000000018"), "uint8"));
    EXPECT_EQ("123456", ABI::ValueDecoder::decodeValue(parse_hex("000000000000000000000000000000000000000000000000000000000001e240"), "uint256"));
    EXPECT_EQ("0xf784682c82526e245f50975190ef0fff4e4fc077", ABI::ValueDecoder::decodeValue(parse_hex("000000000000000000000000f784682c82526e245f50975190ef0fff4e4fc077"), "address"));
    EXPECT_EQ("Hello World!    Hello World!    Hello World!",
              ABI::ValueDecoder::decodeValue(parse_hex(
                                                 "000000000000000000000000000000000000000000000000000000000000002c"
                                                 "48656c6c6f20576f726c64212020202048656c6c6f20576f726c642120202020"
                                                 "48656c6c6f20576f726c64210000000000000000000000000000000000000000"),
                                             "string"));
    EXPECT_EQ("0x31323334353637383930", ABI::ValueDecoder::decodeValue(parse_hex("3132333435363738393000000000000000000000000000000000000000000000"), "bytes10"));
}

TEST(EthereumAbiValueDecoder, decodeArray) {
    {
        // Array of UInt8
        Data input = parse_hex(
            "0000000000000000000000000000000000000000000000000000000000000003"
            "0000000000000000000000000000000000000000000000000000000000000031"
            "0000000000000000000000000000000000000000000000000000000000000032"
            "0000000000000000000000000000000000000000000000000000000000000033");
        auto res = ABI::ValueDecoder::decodeArray(input, "uint8[]");
        EXPECT_EQ(3ul, res.size());
        EXPECT_EQ("49", res[0]);
        EXPECT_EQ("50", res[1]);
        EXPECT_EQ("51", res[2]);
    }
    {
        // Array of Address
        Data input = parse_hex(
            "0000000000000000000000000000000000000000000000000000000000000002"
            "000000000000000000000000f784682c82526e245f50975190ef0fff4e4fc077"
            "0000000000000000000000002e00cd222cb42b616d86d037cc494e8ab7f5c9a3");
        auto res = ABI::ValueDecoder::decodeArray(input, "address[]");
        EXPECT_EQ(2ul, res.size());
        EXPECT_EQ("0xf784682c82526e245f50975190ef0fff4e4fc077", res[0]);
        EXPECT_EQ("0x2e00cd222cb42b616d86d037cc494e8ab7f5c9a3", res[1]);
    }
    {
        // Array of ByteArray
        Data input = parse_hex(
            "0000000000000000000000000000000000000000000000000000000000000002"
            "0000000000000000000000000000000000000000000000000000000000000040"
            "0000000000000000000000000000000000000000000000000000000000000080"
            "0000000000000000000000000000000000000000000000000000000000000002"
            "1011000000000000000000000000000000000000000000000000000000000000"
            "0000000000000000000000000000000000000000000000000000000000000003"
            "1022220000000000000000000000000000000000000000000000000000000000");
        auto res = ABI::ValueDecoder::decodeArray(input, "bytes[]");
        EXPECT_EQ(2ul, res.size());
        EXPECT_EQ("0x1011", res[0]);
        EXPECT_EQ("0x102222", res[1]);
    }
}

} // namespace TW::Ethereum::tests
