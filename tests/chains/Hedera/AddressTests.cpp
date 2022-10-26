// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Hedera/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
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
    }

    {
        // 0.0.302a300506032b65700321007df3e1ab790b28de4706d36a7aa99a0e043cb3e2c3d6ec6686e4af7f638b0860
        // https://github.com/hashgraph/hedera-sdk-rust/blob/c1c10d5750552e6bb857132cc824c430bd890a6b/sdk/rust/src/key/public_key/mod.rs#L306
        auto pubkey = PublicKey(parse_hex("7df3e1ab790b28de4706d36a7aa99a0e043cb3e2c3d6ec6686e4af7f638b0860"), TWPublicKeyTypeED25519);
        Address addr(0uL, 0uL, 0uL, pubkey);
        ASSERT_EQ(addr.shard(), 0uL);
        ASSERT_EQ(addr.realm(), 0uL);
        ASSERT_EQ(addr.num(), 0uL);
        ASSERT_EQ(hex(addr.alias().value().bytesWithHederaDerPrefix()), "302a300506032b65700321007df3e1ab790b28de4706d36a7aa99a0e043cb3e2c3d6ec6686e4af7f638b0860");
        ASSERT_EQ(addr.string(), "0.0.302a300506032b65700321007df3e1ab790b28de4706d36a7aa99a0e043cb3e2c3d6ec6686e4af7f638b0860");
    }
}

/*TEST(HederaAddress, Valid) {
    ASSERT_TRUE(Address::isValid("__ADD_VALID_ADDRESS_HERE__"));

    // TODO: Add more tests
}

TEST(HederaAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("__ADD_INVALID_ADDRESS_HERE__"));

    // TODO: Add more tests
}

TEST(HederaAddress, FromPrivateKey) {
    // TODO: Check public key type, finalize implementation

    auto privateKey = PrivateKey(parse_hex("__PRIVATE_KEY_DATA__"));
    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeED25519));
    ASSERT_EQ(address.string(), "__ADD_RESULTING_ADDRESS_HERE__");
}

TEST(HederaAddress, FromPublicKey) {
    // TODO: Check public key type, finalize implementation
    
    auto publicKey = PublicKey(parse_hex("__PUBLIC_KEY_DATA__"), TWPublicKeyTypeED25519);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "__ADD_RESULTING_ADDRESS_HERE__");
}

TEST(HederaAddress, FromString) {
    auto address = Address("__ADD_VALID_ADDRESS_HERE__");
    ASSERT_EQ(address.string(), "__ADD_SAME_VALID_ADDRESS_HERE__");
}*/

} // namespace TW::Hedera::tests
