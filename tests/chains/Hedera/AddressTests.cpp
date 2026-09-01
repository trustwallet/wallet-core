// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "Hedera/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"

#include "TestUtilities.h"

#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>

namespace TW::Hedera::tests {

TEST(HederaAddress, FromStandardArgument) {
    {
        // 0.0.1377988
        Address addr(0uL, 0uL, 1'377'988uL);
        ASSERT_EQ(addr.shard(), 0uL);
        ASSERT_EQ(addr.realm(), 0uL);
        ASSERT_EQ(addr.num(), 1'377'988uL);
        ASSERT_EQ(addr.string(), "0.0.1377988");
        ASSERT_TRUE(addr.isValid(addr.string()));
    }

    {
        // 0.0.302a300506032b65700321007df3e1ab790b28de4706d36a7aa99a0e043cb3e2c3d6ec6686e4af7f638b0860
        // https://github.com/hashgraph/hedera-sdk-rust/blob/c1c10d5750552e6bb857132cc824c430bd890a6b/sdk/rust/src/key/public_key/mod.rs#L306
        auto pubkey = PublicKey(parse_hex("7df3e1ab790b28de4706d36a7aa99a0e043cb3e2c3d6ec6686e4af7f638b0860"), TWPublicKeyTypeED25519);
        Address addr(0uL, 0uL, 0uL, pubkey);
        ASSERT_EQ(addr.shard(), 0uL);
        ASSERT_EQ(addr.realm(), 0uL);
        ASSERT_EQ(addr.num(), 0uL);
        ASSERT_EQ(addr.alias().string(), "302a300506032b65700321007df3e1ab790b28de4706d36a7aa99a0e043cb3e2c3d6ec6686e4af7f638b0860");
        ASSERT_EQ(addr.string(), "0.0.302a300506032b65700321007df3e1ab790b28de4706d36a7aa99a0e043cb3e2c3d6ec6686e4af7f638b0860");
        ASSERT_TRUE(addr.isValid(addr.string()));
    }

    {
        // Alias form parsed back from its string representation round-trips.
        auto addr = Address("0.0.302a300506032b65700321007df3e1ab790b28de4706d36a7aa99a0e043cb3e2c3d6ec6686e4af7f638b0860");
        ASSERT_EQ(addr.shard(), 0uL);
        ASSERT_EQ(addr.realm(), 0uL);
        ASSERT_EQ(addr.num(), 0uL);
        ASSERT_EQ(addr.alias().string(), "302a300506032b65700321007df3e1ab790b28de4706d36a7aa99a0e043cb3e2c3d6ec6686e4af7f638b0860");
        ASSERT_EQ(addr.string(), "0.0.302a300506032b65700321007df3e1ab790b28de4706d36a7aa99a0e043cb3e2c3d6ec6686e4af7f638b0860");
    }

    {
        // Checksum suffix is parsed as the plain numeric entity ID.
        auto addr = Address("0.0.1377988-abcde");
        ASSERT_EQ(addr.num(), 1377988uL);
    }
}

TEST(HederaAddress, Valid) {
    ASSERT_FALSE(Address::isValid("invalid"));
    ASSERT_FALSE(Address::isValid("302a300506032b65700321007df3e1ab790b28de4706d36a7aa99a0e043cb3e2c3d6ec6686e4af7f638b0860"));
    ASSERT_FALSE(Address::isValid("0.0.abc"));
    // Digits alone are not enough: the components must fit the shard/realm/num fields,
    // so that isValid() accepts exactly what the string constructor can build.
    ASSERT_FALSE(Address::isValid("0.0.99999999999999999999999"));
    ASSERT_FALSE(Address::isValid("99999999999999999999999.0.1"));
    // Components are serialised as signed int64, so INT64_MAX is the last accepted value.
    ASSERT_TRUE(Address::isValid("0.0.9223372036854775807"));
    ASSERT_FALSE(Address::isValid("0.0.9223372036854775808"));
    ASSERT_TRUE(Address::isValid("0.0.1"));
    ASSERT_TRUE(Address::isValid("0.0.1377988"));
    ASSERT_TRUE(Address::isValid("0.0.302a300506032b65700321007df3e1ab790b28de4706d36a7aa99a0e043cb3e2c3d6ec6686e4af7f638b0860"));
}

TEST(HederaAddress, FromString) {
    auto address = Address("0.0.1377988");
    ASSERT_EQ(address.string(), "0.0.1377988");
}

TEST(HederaAddress, FromInvalidString) {
    EXPECT_THROW(Address("invalid"), std::invalid_argument);
    // Well-formed but out of range for std::size_t, so the numeric parse must fail
    // rather than wrap around.
    EXPECT_THROW(Address("0.0.99999999999999999999999"), std::invalid_argument);
}

} // namespace TW::Hedera::tests
