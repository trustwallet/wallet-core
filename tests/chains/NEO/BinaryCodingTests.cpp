// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "NEO/BinaryCoding.h"

#include <gtest/gtest.h>

namespace TW::NEO::tests {

using namespace std;

TEST(NEOBinaryCoding, encode256LE) {
    Data d;
    encode256LE(d, uint256_t(110000000));
    EXPECT_EQ(hex(d), "80778e06");
}

TEST(NEOBinaryCoding, encode256LEWithPadding) {
    Data d;
    encode256LE(d, uint256_t(10000000));
    EXPECT_EQ(hex(d), "80969800");
}

TEST(NEOBinaryCoding, encodeBytes) {
    Data d;
    Data value;
    std::fill_n(std::back_inserter(value), 10, 'a');
    encodeBytes(d, value);
    EXPECT_EQ(d.size(), value.size() + 1);

    d.clear();
    value.clear();
    std::fill_n(std::back_inserter(value), 255, 'a');
    encodeBytes(d, value);
    EXPECT_EQ(d.size(), value.size() + 1 + 1);

    d.clear();
    value.clear();
    std::fill_n(std::back_inserter(value), 300, 'a');
    encodeBytes(d, value);
    EXPECT_EQ(d.size(), value.size() + 1 + 2);

    d.clear();
    value.clear();
    std::fill_n(std::back_inserter(value), 0x10000, 'a');
    encodeBytes(d, value);
    EXPECT_EQ(d.size(), value.size() + 1 + 4);
}

} // namespace TW::NEO::tests
