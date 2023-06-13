// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Data.h"
#include "HexCoding.h"
#include "Substrate/Address.h"
#include "Substrate/ScaleCodec.h"
#include "uint256.h"
#include <TrustWalletCore/TWSS58AddressType.h>

#include <gtest/gtest.h>

using namespace std;
using namespace TW;

namespace TW::Substrate::tests {

TEST(SubstrateScaleCodec, CountBytes) {
    EXPECT_EQ(size_t(1), countBytes(uint256_t(0)));
    EXPECT_EQ(size_t(1), countBytes(uint256_t(1)));
    EXPECT_EQ(size_t(2), countBytes(uint256_t("0x1ff")));
}

TEST(SubstrateScaleCodec, EncodeEra) {
    uint64_t block = 4246319;
    uint64_t period = 64;
    auto data = encodeEra(block, period);
    EXPECT_EQ(hex(data), "f502");
}

TEST(SubstrateScaleCodec, EncodeVectorAccountIds) {
    auto addresses = std::vector<SS58Address>{
        SS58Address("FoQJpPyadYccjavVdTWxpxU7rUEaYhfLCPwXgkfD6Zat9QP", TWSS58AddressTypeKusama),
        SS58Address("CtwdfrhECFs3FpvCGoiE4hwRC4UsSiM8WL899HjRdQbfYZY", TWSS58AddressTypeKusama)};
    auto encoded = encodeAccountIds(addresses, false);
    ASSERT_EQ(hex(encoded), "08008eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48000e33fdfb980e4499e5c3576e742a563b6a4fc0f6f598b1917fd7a6fe393ffc72");
}

TEST(SubstrateScaleCodec, EncodeCompact) {
    ASSERT_EQ(hex(encodeCompact(0)), "00");
    ASSERT_EQ(hex(encodeCompact(18)), "48");
    ASSERT_EQ(hex(encodeCompact(63)), "fc");
    ASSERT_EQ(hex(encodeCompact(64)), "0101");

    ASSERT_EQ(hex(encodeCompact(12345)), "e5c0");
    ASSERT_EQ(hex(encodeCompact(16383)), "fdff");
    ASSERT_EQ(hex(encodeCompact(16384)), "02000100");

    ASSERT_EQ(hex(encodeCompact(1073741823)), "feffffff");
    ASSERT_EQ(hex(encodeCompact(1073741824)), "0300000040");

    ASSERT_EQ(hex(encodeCompact(4294967295)), "03ffffffff");
    ASSERT_EQ(hex(encodeCompact(4294967296)), "070000000001");

    ASSERT_EQ(hex(encodeCompact(1099511627776)), "0b000000000001");
    ASSERT_EQ(hex(encodeCompact(281474976710656)), "0f00000000000001");

    ASSERT_EQ(hex(encodeCompact(72057594037927935)), "0fffffffffffffff");
    ASSERT_EQ(hex(encodeCompact(72057594037927936)), "130000000000000001");

    ASSERT_EQ(hex(encodeCompact(18446744073709551615u)), "13ffffffffffffffff");
}

} // namespace TW::Substrate::tests