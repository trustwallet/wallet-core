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
    // Address 20 are valid in SUI
    ASSERT_TRUE(Address::isValid("0xb1dc06bd64d4e179a482b97bb68243f6c02c1b92"));
    ASSERT_TRUE(Address::isValid("b1dc06bd64d4e179a482b97bb68243f6c02c1b92"));
}

TEST(SuiAddress, Invalid) {
    // Address 32 are invalid in SUI
    ASSERT_FALSE(Address::isValid("0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b"));
    ASSERT_FALSE(Address::isValid("eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b"));
    ASSERT_FALSE(Address::isValid("19aadeca9388e009d136245b9a67423f3eee242b03142849eb4f81a4a409e59c"));
    // Too long
    ASSERT_FALSE(Address::isValid("b1dc06bd64d4e179a482b97bb68243f6c02c1b921"));
    // Too short
    ASSERT_FALSE(Address::isValid("b1dc06bd64d4e179a482b97bb68243f6c02c1b9"));
    // Invalid Hex
    ASSERT_FALSE(Address::isValid("0xS1dc06bd64d4e179a482b97bb68243f6c02c1b92"));
}

TEST(SuiAddress, FromString) {
    auto address = Address("b1dc06bd64d4e179a482b97bb68243f6c02c1b92");
    ASSERT_EQ(address.string(), "0xb1dc06bd64d4e179a482b97bb68243f6c02c1b92");
}

TEST(SuiAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("088baa019f081d6eab8dff5c447f9ce2f83c1babf3d03686299eaf6a1e89156e"));
    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeED25519));
    ASSERT_EQ(address.string(), "0xb638d15fa81d301a9756259d0c7b2ca27a00a531");
}

TEST(SuiAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("ad0e293a56c9fc648d1872a00521d97e6b65724519a2676c2c47cb95d131cf5a"), TWPublicKeyTypeED25519);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "0xb638d15fa81d301a9756259d0c7b2ca27a00a531");
}

} // namespace TW::Sui::tests
