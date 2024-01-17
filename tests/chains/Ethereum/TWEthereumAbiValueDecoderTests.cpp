// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWEthereumAbiValue.h>

#include "Data.h"
#include "HexCoding.h"
#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace std;

TEST(TWEthereumAbiValue, decodeUInt256) {
    const char * expected = "10020405371567";
    auto inputs = vector<string>{
        "091d0eb3e2af",
        "0000000000000000000000000000000000000000000000000000091d0eb3e2af",
        "0000000000000000000000000000000000000000000000000000091d0eb3e2af0000000000000000000000000000000000000000000000000000000000000000",
        "0000000000000000000000000000000000000000000000000000091d0eb3e2af00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
    };
    for (auto& input: inputs) {
        auto data =  WRAPD(TWDataCreateWithHexString(STRING(input.c_str()).get()));
        auto result = WRAPS(TWEthereumAbiValueDecodeUInt256(data.get()));
        assertStringsEqual(result, expected);
    }
}

TEST(TWEthereumAbiValue, decodeValue) {
    {
        const auto input = "000000000000000000000000000000000000000000000000000000000000002a";
        const auto type = "uint";
        const auto expected = "42";
        auto data = WRAPD(TWDataCreateWithHexString(STRING(input).get()));
        auto result = WRAPS(TWEthereumAbiValueDecodeValue(data.get(), WRAPS(TWStringCreateWithUTF8Bytes(type)).get()));
        EXPECT_EQ(std::string(expected), std::string(TWStringUTF8Bytes(result.get())));
    }
    {
        const auto input = "0000000000000000000000000000000000000000000000000000000000000018";
        const auto type = "uint8";
        const auto expected = "24";
        auto data = WRAPD(TWDataCreateWithHexString(STRING(input).get()));
        auto result = WRAPS(TWEthereumAbiValueDecodeValue(data.get(), WRAPS(TWStringCreateWithUTF8Bytes(type)).get()));
        EXPECT_EQ(std::string(expected), std::string(TWStringUTF8Bytes(result.get())));
    }
    {
        const auto input = "000000000000000000000000f784682c82526e245f50975190ef0fff4e4fc077";
        const auto type = "address";
        const auto expected = "0xF784682C82526e245F50975190EF0fff4E4fC077";
        auto data = WRAPD(TWDataCreateWithHexString(STRING(input).get()));
        auto result = WRAPS(TWEthereumAbiValueDecodeValue(data.get(), WRAPS(TWStringCreateWithUTF8Bytes(type)).get()));
        EXPECT_EQ(std::string(expected), std::string(TWStringUTF8Bytes(result.get())));
    }
    {
        const auto input =
            "000000000000000000000000000000000000000000000000000000000000002c"
            "48656c6c6f20576f726c64212020202048656c6c6f20576f726c642120202020"
            "48656c6c6f20576f726c64210000000000000000000000000000000000000000";
        const auto type = "string";
        const auto expected = "Hello World!    Hello World!    Hello World!";
        auto data = WRAPD(TWDataCreateWithHexString(STRING(input).get()));
        auto result = WRAPS(TWEthereumAbiValueDecodeValue(data.get(), WRAPS(TWStringCreateWithUTF8Bytes(type)).get()));
        EXPECT_EQ(std::string(expected), std::string(TWStringUTF8Bytes(result.get())));
    }
    {
        const auto input = "3132333435363738393000000000000000000000000000000000000000000000";
        const auto type = "bytes10";
        const auto expected = "0x31323334353637383930";
        auto data = WRAPD(TWDataCreateWithHexString(STRING(input).get()));
        auto result = WRAPS(TWEthereumAbiValueDecodeValue(data.get(), WRAPS(TWStringCreateWithUTF8Bytes(type)).get()));
        EXPECT_EQ(std::string(expected), std::string(TWStringUTF8Bytes(result.get())));
    }
}

TEST(TWEthereumAbiValue, decodeArray) {
    {
        // Array of UInt8
        const auto input =
            "0000000000000000000000000000000000000000000000000000000000000003"
            "0000000000000000000000000000000000000000000000000000000000000031"
            "0000000000000000000000000000000000000000000000000000000000000032"
            "0000000000000000000000000000000000000000000000000000000000000033";
        const auto type = "uint8[]";
        const auto expected = "[49,50,51]";
        auto data = WRAPD(TWDataCreateWithHexString(STRING(input).get()));
        auto result = WRAPS(TWEthereumAbiValueDecodeArray(data.get(), WRAPS(TWStringCreateWithUTF8Bytes(type)).get()));
        EXPECT_EQ(std::string(expected), std::string(TWStringUTF8Bytes(result.get())));
    }
    {
        // Array of Address
        const auto input =
            "0000000000000000000000000000000000000000000000000000000000000002"
            "000000000000000000000000f784682c82526e245f50975190ef0fff4e4fc077"
            "0000000000000000000000002e00cd222cb42b616d86d037cc494e8ab7f5c9a3";
        const auto type = "address[]";
        const auto expected = 
            "[\"0xF784682C82526e245F50975190EF0fff4E4fC077\","
            "\"0x2e00CD222Cb42B616D86D037Cc494e8ab7F5c9a3\"]";
        auto data = WRAPD(TWDataCreateWithHexString(STRING(input).get()));
        auto result = WRAPS(TWEthereumAbiValueDecodeArray(data.get(), WRAPS(TWStringCreateWithUTF8Bytes(type)).get()));
        EXPECT_EQ(std::string(expected), std::string(TWStringUTF8Bytes(result.get())));
    }
    {
        // Array of ByteArray
        const auto input =
            "0000000000000000000000000000000000000000000000000000000000000002"
            "0000000000000000000000000000000000000000000000000000000000000040"
            "0000000000000000000000000000000000000000000000000000000000000080"
            "0000000000000000000000000000000000000000000000000000000000000002"
            "1011000000000000000000000000000000000000000000000000000000000000"
            "0000000000000000000000000000000000000000000000000000000000000003"
            "1022220000000000000000000000000000000000000000000000000000000000";
        const auto type = "bytes[]";
        const auto expected = "[\"0x1011\",\"0x102222\"]";
        auto data = WRAPD(TWDataCreateWithHexString(STRING(input).get()));
        auto result = WRAPS(TWEthereumAbiValueDecodeArray(data.get(), WRAPS(TWStringCreateWithUTF8Bytes(type)).get()));
        EXPECT_EQ(std::string(expected), std::string(TWStringUTF8Bytes(result.get())));
    }
}
