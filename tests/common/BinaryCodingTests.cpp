// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "BinaryCoding.h"
#include "HexCoding.h"

#include <vector>

#include <gtest/gtest.h>

using namespace std;
using namespace TW;

TEST(BinaryCodingTests, varIntSize) {
    vector<tuple<uint64_t, uint8_t>> tests = {
        {0, 1},
        {1, 1},
        {10, 1},
        {100, 1},
        {0xfb, 1},
        {0xfc, 1},
        {0xfd, 3},
        {0xfe, 3},
        {0xff, 3},
        {0x100, 3},
        {0x200, 3},
        {0x1000, 3},
        {0xffff, 3},
        {0x10000, 5},
        {0x20000, 5},
        {0xffffffff, 5},
        {0x100000000, 9},
        {0x200000000, 9},
        {0x1000000000, 9},
        {0x10000000000, 9},
        {0x100000000000, 9},
        {0x1000000000000, 9},
        {0x10000000000000, 9},
        {0x100000000000000, 9},
        {0xffffffffffffffff, 9},
    };
    for (auto& test : tests) {
        EXPECT_EQ(varIntSize(get<0>(test)), get<1>(test));
    }
}

TEST(BinaryCodingTests, encodeAndDecodeVarInt) {
    vector<tuple<uint64_t, string>> tests = {
        {0, "00"},
        {1, "01"},
        {10, "0a"},
        {100, "64"},
        {0xfb, "fb"},
        {0xfc, "fc"},
        {0xfd, "fdfd00"},
        {0xfe, "fdfe00"},
        {0xff, "fdff00"},
        {0x100, "fd0001"},
        {0x200, "fd0002"},
        {0x1000, "fd0010"},
        {0xffff, "fdffff"},
        {0x10000, "fe00000100"},
        {0x20000, "fe00000200"},
        {0xffffffff, "feffffffff"},
        {0x100000000, "ff0000000001000000"},
        {0x200000000, "ff0000000002000000"},
        {0x1000000000, "ff0000000010000000"},
        {0x10000000000, "ff0000000000010000"},
        {0x100000000000, "ff0000000000100000"},
        {0x1000000000000, "ff0000000000000100"},
        {0x10000000000000, "ff0000000000001000"},
        {0x100000000000000, "ff0000000000000001"},
        {0xffffffffffffffff, "ffffffffffffffffff"},
    };
    for (auto& test : tests) {
        const auto input = get<0>(test);
        Data encoded;
        uint8_t resultEnc = encodeVarInt(input, encoded);
        EXPECT_EQ(hex(encoded), get<1>(test));
        EXPECT_EQ(resultEnc, varIntSize(input));
        // decode back
        size_t index = 0;
        const auto resultDec = decodeVarInt(encoded, index);
        EXPECT_EQ(get<0>(resultDec), true);
        EXPECT_EQ(get<1>(resultDec), input);
    }
}

TEST(BinaryCodingTests, decodeVarIntTooShort) {
    {
        Data encoded = parse_hex("fe000000"); // one byte missing
        size_t index = 0;
        const auto result = decodeVarInt(encoded, index);
        EXPECT_EQ(get<0>(result), false);
    }
    {
        Data encoded = parse_hex("fe00000000");
        size_t index = 0;
        const auto result = decodeVarInt(encoded, index);
        EXPECT_EQ(get<0>(result), true);
    }
}

TEST(BinaryCodingTests, encodeAndDecodeString) {
    vector<tuple<string, string>> tests = {
        {"", "00"},
        {"A", "0141"},
        {"AB", "024142"},
        {"abcdefghij", "0a6162636465666768696a"},
        {"abcdefghIj", "0a6162636465666768496a"},
        {"12345678901234567890123456789012345678901234567890", "323132333435363738393031323334353637383930313233343536373839303132333435363738393031323334353637383930"},
        {
            "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
            "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
            "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
            , "fd2c01"
            "3132333435363738393031323334353637383930313233343536373839303132333435363738393031323334353637383930"
            "3132333435363738393031323334353637383930313233343536373839303132333435363738393031323334353637383930"
            "3132333435363738393031323334353637383930313233343536373839303132333435363738393031323334353637383930"
            "3132333435363738393031323334353637383930313233343536373839303132333435363738393031323334353637383930"
            "3132333435363738393031323334353637383930313233343536373839303132333435363738393031323334353637383930"
            "3132333435363738393031323334353637383930313233343536373839303132333435363738393031323334353637383930"
        },
    };
    for (auto& test : tests) {
        const auto input = get<0>(test);
        Data encoded;
        encodeString(input, encoded);
        EXPECT_EQ(hex(encoded), get<1>(test));
        // decode back
        size_t index = 0;
        const auto resultDec = decodeString(encoded, index);
        EXPECT_EQ(get<0>(resultDec), true);
        EXPECT_EQ(get<1>(resultDec), get<0>(test));
    }
}

TEST(BinaryCodingTests, decodeStringTooShort) {
    {
        Data encoded = parse_hex("0a616263646566676849"); // one byte missing
        size_t index = 0;
        const auto result = decodeString(encoded, index);
        EXPECT_EQ(get<0>(result), false);
    }
    {
        Data encoded = parse_hex("0a6162636465666768496a");
        size_t index = 0;
        const auto result = decodeString(encoded, index);
        EXPECT_EQ(get<0>(result), true);
    }
}
