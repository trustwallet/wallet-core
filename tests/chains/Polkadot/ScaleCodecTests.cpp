// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.


#include "HexCoding.h"
#include "Polkadot/ScaleCodec.h"
#include "Kusama/Address.h"
#include "uint256.h"

#include <gtest/gtest.h>

using namespace TW;
namespace TW::Polkadot::tests {

TEST(PolkadotCodec, EncodeCompact) {
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

TEST(PolkadotCodec, EncodeBool) {
    ASSERT_EQ(hex(encodeBool(true)), "01");    
    ASSERT_EQ(hex(encodeBool(false)), "00");
}

TEST(PolkadotCodec, EncodeLengthPrefix) {
    auto encoded = parse_hex("84ff88dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee0034a113577b56545c45e18969471eebe11ed434f3b2f06e2e3dc8dc137ba804caf60757787ebdeb298327e2f29d68c5520965405ef5582db0445c06e1c11a8a0e0000000400ff8eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c0");
    encodeLengthPrefix(encoded);

    ASSERT_EQ(hex(encoded), "2d0284ff88dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee0034a113577b56545c45e18969471eebe11ed434f3b2f06e2e3dc8dc137ba804caf60757787ebdeb298327e2f29d68c5520965405ef5582db0445c06e1c11a8a0e0000000400ff8eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c0");
}

TEST(PolkadotCodec, encodeAccountId) {
    auto address = Kusama::Address("FoQJpPyadYccjavVdTWxpxU7rUEaYhfLCPwXgkfD6Zat9QP");
    auto encoded = encodeAccountId(address.keyBytes(), true);
    auto encoded2 = encodeAccountId(address.keyBytes(), false);

    ASSERT_EQ(hex(encoded), "8eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48");
    ASSERT_EQ(hex(encoded2), "008eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48");
}

TEST(PolkadotCodec, EncodeVectorAccountIds) {
    auto addresses = std::vector<SS58Address>{
        Kusama::Address("FoQJpPyadYccjavVdTWxpxU7rUEaYhfLCPwXgkfD6Zat9QP"),
        Kusama::Address("CtwdfrhECFs3FpvCGoiE4hwRC4UsSiM8WL899HjRdQbfYZY")
    };
    auto encoded = encodeAccountIds(addresses, false);
    ASSERT_EQ(hex(encoded), "08008eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48000e33fdfb980e4499e5c3576e742a563b6a4fc0f6f598b1917fd7a6fe393ffc72");
}

TEST(PolkadotCodec, EncodeVectorAccountIdsKusama) {
    auto addresses = std::vector<SS58Address>{
        SS58Address("FoQJpPyadYccjavVdTWxpxU7rUEaYhfLCPwXgkfD6Zat9QP", TWSS58AddressTypeKusama),
        SS58Address("CtwdfrhECFs3FpvCGoiE4hwRC4UsSiM8WL899HjRdQbfYZY", TWSS58AddressTypeKusama)};
    auto encoded = encodeAccountIds(addresses, false);
    ASSERT_EQ(hex(encoded), "08008eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48000e33fdfb980e4499e5c3576e742a563b6a4fc0f6f598b1917fd7a6fe393ffc72");
}

TEST(PolkadotCodec, EncodeEra) {
    auto era1 = encodeEra(429119, 8);
    auto era2 = encodeEra(428861, 4);
    auto era3 = encodeEra(4246319, 64);
    ASSERT_EQ(hex(era1), "7200");
    ASSERT_EQ(hex(era2), "1100");
    EXPECT_EQ(hex(era3), "f502");
}

TEST(PolkadotCodec, CountBytes) {
    EXPECT_EQ(size_t(1), countBytes(uint256_t(0)));
    EXPECT_EQ(size_t(1), countBytes(uint256_t(1)));
    EXPECT_EQ(size_t(2), countBytes(uint256_t("0x1ff")));
}

} // namespace TW::Polkadot::tests
