// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Aion/RLP.h"
#include "HexCoding.h"
#include <gtest/gtest.h>

namespace TW::Aion::tests {

using boost::multiprecision::uint128_t;

// Function helper over `RLP::prepareLong`.
Data encodeLong(const uint128_t& l) {
    EthereumRlp::Proto::EncodingInput input;
    *input.mutable_item() = RLP::prepareLong(l);
    return Ethereum::RLP::encode(input);
}

TEST(AionRLP, EncodeLong) {
    EXPECT_EQ(hex(encodeLong(uint128_t(1))), "01");
    EXPECT_EQ(hex(encodeLong(uint128_t(21000))), "825208");
    EXPECT_EQ(hex(encodeLong(uint128_t(1000000))), "830f4240");
    EXPECT_EQ(hex(encodeLong(uint128_t(20000000000))), "8800000004a817c800");
    EXPECT_EQ(hex(encodeLong(uint128_t(9007199254740991))), "88001fffffffffffff");
    EXPECT_EQ(hex(encodeLong(uint128_t(9007199254740990))), "88001ffffffffffffe");
    EXPECT_EQ(hex(encodeLong(uint128_t(4294967296L))), "880000000100000000");
    EXPECT_EQ(hex(encodeLong(uint128_t(4295000060L))), "880000000100007ffc");
    EXPECT_EQ(hex(encodeLong(uint128_t(72057594037927935L))), "8800ffffffffffffff");
}

} // namespace TW::Aion::tests
