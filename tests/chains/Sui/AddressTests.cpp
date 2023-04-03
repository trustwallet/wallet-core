// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Sui/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

namespace TW::Sui::tests {

TEST(SuiAddress, Valid) {
    ASSERT_TRUE(Address::isValid("0x1"));
    // Address 32 are valid in SUI
    ASSERT_TRUE(Address::isValid("0x259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015"));
    ASSERT_TRUE(Address::isValid("259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015"));
}

TEST(SuiAddress, Invalid) {
    // Address 20 are invalid in SUI
    ASSERT_FALSE(Address::isValid("0xb1dc06bd64d4e179a482b97bb68243f6c02c1b92"));
    ASSERT_FALSE(Address::isValid("b1dc06bd64d4e179a482b97bb68243f6c02c1b92"));
    // Too long
    ASSERT_FALSE(Address::isValid("d575ad7f18e948462a5cf698f564ef394a752a71fec62493af8a055c012c0d502"));
    // Too short
    ASSERT_FALSE(Address::isValid("b1dc06bd64d4e179a482b97bb68243f6c02c1b9"));
    // Invalid short address
    ASSERT_FALSE(Address::isValid("0x11"));
    // Invalid Hex
    ASSERT_FALSE(Address::isValid("0xS59ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015"));
}

TEST(SuiAddress, FromString) {
    auto address = Address("259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015");
    ASSERT_EQ(address.string(), "0x259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015");
}

TEST(SuiAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("088baa019f081d6eab8dff5c447f9ce2f83c1babf3d03686299eaf6a1e89156e"));
    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeED25519));
    ASSERT_EQ(address.string(), "0x259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015");
}

TEST(SuiAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("ad0e293a56c9fc648d1872a00521d97e6b65724519a2676c2c47cb95d131cf5a"), TWPublicKeyTypeED25519);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "0x259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015");
}

} // namespace TW::Sui::tests
