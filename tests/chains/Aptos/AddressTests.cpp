// Copyright © 2017-2023 Trust Wallet.
// Author: Clement Doumergue
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Aptos/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

namespace TW::Aptos::tests {

TEST(AptosAddress, Valid) {
    ASSERT_TRUE(Address::isValid("0x1"));
    ASSERT_TRUE(Address::isValid(Address::one().string()));
    ASSERT_TRUE(Address::isValid(Address::zero().string()));
    ASSERT_TRUE(Address::isValid("0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b"));
    ASSERT_TRUE(Address::isValid("eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b"));
    ASSERT_TRUE(Address::isValid("19aadeca9388e009d136245b9a67423f3eee242b03142849eb4f81a4a409e59c"));
    ASSERT_TRUE(Address::isValid("777821c78442e17d82c3d7a371f42de7189e4248e529fe6eee6bca40ddbb"));
    ASSERT_TRUE(Address::isValid("0x777821c78442e17d82c3d7a371f42de7189e4248e529fe6eee6bca40ddbb"));
    ASSERT_TRUE(Address::isValid("eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175"));   // too short -> automatically padded
}

TEST(AptosAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("Seff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b"));  // Invalid hex character
    ASSERT_FALSE(Address::isValid("eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175bb")); // Invalid length: too long

}

TEST(AptosAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("088baa019f081d6eab8dff5c447f9ce2f83c1babf3d03686299eaf6a1e89156e"));
    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeED25519));
    ASSERT_EQ(address.string(), "0xe9c4d0b6fe32a5cc8ebd1e9ad5b54a0276a57f2d081dcb5e30342319963626c3");
}

TEST(AptosAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("ad0e293a56c9fc648d1872a00521d97e6b65724519a2676c2c47cb95d131cf5a"), TWPublicKeyTypeED25519);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "0xe9c4d0b6fe32a5cc8ebd1e9ad5b54a0276a57f2d081dcb5e30342319963626c3");
}

TEST(AptosAddress, FromString) {
    auto address = Address("eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b");
    ASSERT_EQ(address.string(), "0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b");


    address = Address("0x777821c78442e17d82c3d7a371f42de7189e4248e529fe6eee6bca40ddbb");
    ASSERT_EQ(address.string(), "0x0000777821c78442e17d82c3d7a371f42de7189e4248e529fe6eee6bca40ddbb");
    address = Address("777821c78442e17d82c3d7a371f42de7189e4248e529fe6eee6bca40ddbb");
    ASSERT_EQ(address.string(), "0x0000777821c78442e17d82c3d7a371f42de7189e4248e529fe6eee6bca40ddbb");
}

TEST(AptosAddress, ShortString) {
    ASSERT_EQ(Address::one().string(), "0x0000000000000000000000000000000000000000000000000000000000000001");
    ASSERT_EQ(Address::one().shortString(), "1");
}

} // namespace TW::Aptos::tests
