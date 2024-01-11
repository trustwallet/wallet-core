// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "Hedera/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"

#include "TestUtilities.h"

#include <gtest/gtest.h>
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
}

TEST(HederaAddress, Valid) {
    ASSERT_FALSE(Address::isValid("invalid"));
    ASSERT_FALSE(Address::isValid("302a300506032b65700321007df3e1ab790b28de4706d36a7aa99a0e043cb3e2c3d6ec6686e4af7f638b0860"));
    ASSERT_FALSE(Address::isValid("0.0.abc"));
    ASSERT_TRUE(Address::isValid("0.0.1"));
    ASSERT_TRUE(Address::isValid("0.0.1377988"));
    ASSERT_TRUE(Address::isValid("0.0.302a300506032b65700321007df3e1ab790b28de4706d36a7aa99a0e043cb3e2c3d6ec6686e4af7f638b0860"));
}

TEST(HederaAddress, FromString) {
    auto address = Address("0.0.1377988");
    ASSERT_EQ(address.string(), "0.0.1377988");
}

} // namespace TW::Hedera::tests
