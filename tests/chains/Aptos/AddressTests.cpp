// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// Author: Clement Doumergue
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Aptos/Entry.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

namespace TW::Aptos::tests {

TEST(AptosAddress, Valid) {
    Entry entry;
    ASSERT_TRUE(entry.validateAddress(TWCoinTypeAptos, "0x1", std::monostate{}));
    ASSERT_TRUE(entry.validateAddress(TWCoinTypeAptos, "0x0", std::monostate{}));
    ASSERT_TRUE(entry.validateAddress(TWCoinTypeAptos, "0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b", std::monostate{}));
    ASSERT_TRUE(entry.validateAddress(TWCoinTypeAptos, "eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b", std::monostate{}));
    ASSERT_TRUE(entry.validateAddress(TWCoinTypeAptos, "19aadeca9388e009d136245b9a67423f3eee242b03142849eb4f81a4a409e59c", std::monostate{}));
    ASSERT_TRUE(entry.validateAddress(TWCoinTypeAptos, "0x777821c78442e17d82c3d7a371f42de7189e4248e529fe6eee6bca40ddbb", std::monostate{}));
    ASSERT_TRUE(entry.validateAddress(TWCoinTypeAptos, "0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175", std::monostate{}));
}

TEST(AptosAddress, Invalid) {
    Entry entry;
    ASSERT_FALSE(entry.validateAddress(TWCoinTypeAptos, "", std::monostate{}));
    ASSERT_FALSE(entry.validateAddress(TWCoinTypeAptos, "Seff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b", std::monostate{}));
    ASSERT_FALSE(entry.validateAddress(TWCoinTypeAptos, "eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175bb", std::monostate{}));
    ASSERT_FALSE(entry.validateAddress(TWCoinTypeAptos, "0xSeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b", std::monostate{}));

}

TEST(AptosAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("088baa019f081d6eab8dff5c447f9ce2f83c1babf3d03686299eaf6a1e89156e"));
    auto pubkey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    Entry entry;
    auto address = entry.deriveAddress(TWCoinTypeAptos, pubkey, TWDerivationDefault, std::monostate{});
    ASSERT_EQ(address, "0xe9c4d0b6fe32a5cc8ebd1e9ad5b54a0276a57f2d081dcb5e30342319963626c3");
}

TEST(AptosAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("ad0e293a56c9fc648d1872a00521d97e6b65724519a2676c2c47cb95d131cf5a"), TWPublicKeyTypeED25519);
    Entry entry;
    auto address = entry.deriveAddress(TWCoinTypeAptos, publicKey, TWDerivationDefault, std::monostate{});
    ASSERT_EQ(address, "0xe9c4d0b6fe32a5cc8ebd1e9ad5b54a0276a57f2d081dcb5e30342319963626c3");
}

} // namespace TW::Aptos::tests
