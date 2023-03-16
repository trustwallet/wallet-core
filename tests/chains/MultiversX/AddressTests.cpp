// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <gtest/gtest.h>
#include <vector>

#include "MultiversX/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TestAccounts.h"

using namespace TW;

namespace TW::MultiversX::tests {

TEST(MultiversXAddress, Valid) {
    ASSERT_TRUE(Address::isValid(ALICE_BECH32));
    ASSERT_TRUE(Address::isValid(BOB_BECH32));
}

TEST(MultiversXAddress, Invalid) {
    ASSERT_FALSE(Address::isValid(""));
    ASSERT_FALSE(Address::isValid("foo"));
    ASSERT_FALSE(Address::isValid("10z9xdugayn528ksaesdwlhf006fw5sg2qmmm0h52fvxczwgesyvq5pwemr"));
    ASSERT_FALSE(Address::isValid("xerd10z9xdugayn528ksaesdwlhf006fw5sg2qmmm0h52fvxczwgesyvq5pwemr"));
    ASSERT_FALSE(Address::isValid("foo10z9xdugayn528ksaesdwlhf006fw5sg2qmmm0h52fvxczwgesyvq5pwemr"));
    ASSERT_FALSE(Address::isValid(ALICE_PUBKEY_HEX));
}

TEST(MultiversXAddress, FromString) {
    Address alice, bob, carol;
    ASSERT_TRUE(Address::decode(ALICE_BECH32, alice));
    ASSERT_TRUE(Address::decode(BOB_BECH32, bob));

    ASSERT_EQ(ALICE_PUBKEY_HEX, hex(alice.getKeyHash()));
    ASSERT_EQ(BOB_PUBKEY_HEX, hex(bob.getKeyHash()));
}

TEST(MultiversXAddress, FromData) {
    const auto alice = Address(parse_hex(ALICE_PUBKEY_HEX));
    const auto bob = Address(parse_hex(BOB_PUBKEY_HEX));

    ASSERT_EQ(ALICE_BECH32, alice.string());
    ASSERT_EQ(BOB_BECH32, bob.string());
}

TEST(MultiversXAddress, FromPrivateKey) {
    auto aliceKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    auto alice = Address(aliceKey.getPublicKey(TWPublicKeyTypeED25519));
    ASSERT_EQ(ALICE_BECH32, alice.string());

    auto bobKey = PrivateKey(parse_hex(BOB_SEED_HEX));
    auto bob = Address(bobKey.getPublicKey(TWPublicKeyTypeED25519));
    ASSERT_EQ(BOB_BECH32, bob.string());
}

TEST(MultiversXAddress, FromPublicKey) {
    auto alice = PublicKey(parse_hex(ALICE_PUBKEY_HEX), TWPublicKeyTypeED25519);
    ASSERT_EQ(ALICE_BECH32, Address(alice).string());

    auto bob = PublicKey(parse_hex(BOB_PUBKEY_HEX), TWPublicKeyTypeED25519);
    ASSERT_EQ(BOB_BECH32, Address(bob).string());
}

} // namespace TW::MultiversX::tests
