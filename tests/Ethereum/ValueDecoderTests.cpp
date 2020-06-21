// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ethereum/ABI/ValueDecoder.h"
#include <HexCoding.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Ethereum;

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
