// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "Pactus/Entry.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnyAddress.h>
#include <gtest/gtest.h>

namespace TW::Pactus::tests {

TEST(PactusAddress, AddressData) {
    auto string = STRING("pc1rspm7ps49gar9ft5g0tkl6lhxs8ygeakq87quh3");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypePactus));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "038077e0c2a5474654ae887aedfd7ee681c88cf6c0");
}

TEST(PactusAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("2134ae97465505dfd5a1fd05a8a0f146209c601eb3f1b0363b4cfe4b47ba1ab4"));
    auto pubkey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    Entry entry;
    auto address = entry.deriveAddress(TWCoinTypePactus, pubkey, TWDerivationDefault, std::monostate{});
    ASSERT_EQ(address, "pc1r7jkvfnegf0rf5ua05fzu9krjhjxcrrygl3v4nl");
}

TEST(PactusAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("600d30a4373ae788e2d4a08f4728f45d259593fbdd9632bbe283c4c37ac6a3df"), TWPublicKeyTypeED25519);
    Entry entry;
    auto address = entry.deriveAddress(TWCoinTypePactus, publicKey, TWDerivationDefault, std::monostate{});
    ASSERT_EQ(address, "pc1r7jkvfnegf0rf5ua05fzu9krjhjxcrrygl3v4nl");
}

} // namespace TW::Pactus::tests