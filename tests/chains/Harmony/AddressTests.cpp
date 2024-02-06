// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Harmony/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace TW;

namespace TW::Harmony::tests {

TEST(HarmonyAddress, FromString) {
    Address sender;
    ASSERT_TRUE(Address::decode("one1a50tun737ulcvwy0yvve0pvu5skq0kjargvhwe", sender));
    Address receiver;
    ASSERT_TRUE(Address::decode("one1tp7xdd9ewwnmyvws96au0e7e7mz6f8hjqr3g3p", receiver));

    ASSERT_EQ("ed1ebe4fd1f73f86388f231997859ca42c07da5d", hex(sender.getKeyHash()));
    ASSERT_EQ("587c66b4b973a7b231d02ebbc7e7d9f6c5a49ef2", hex(receiver.getKeyHash()));
}

TEST(HarmonyAddress, FromData) {
    const auto address = Address(parse_hex("0x587c66b4b973a7b231d02ebbc7e7d9f6c5a49ef2"));
    const auto address_2 = Address(parse_hex("0xed1ebe4fd1f73f86388f231997859ca42c07da5d"));
    ASSERT_EQ(address.string(), "one1tp7xdd9ewwnmyvws96au0e7e7mz6f8hjqr3g3p");
    ASSERT_EQ(address_2.string(), "one1a50tun737ulcvwy0yvve0pvu5skq0kjargvhwe");

    EXPECT_ANY_THROW(new Address(parse_hex("")));
}

TEST(HarmonyAddress, InvalidHarmonyAddress) {
    ASSERT_FALSE(Address::isValid("one1a50tun737ulcvwy0yvve0pe"));
    ASSERT_FALSE(Address::isValid("oe1tp7xdd9ewwnmyvws96au0ee7e7mz6f8hjqr3g3p"));
}

TEST(HarmonyAddress, FromPublicKey) {
    const auto privateKey =
        PrivateKey(parse_hex("e2f88b4974ae763ca1c2db49218802c2e441293a09eaa9ab681779e05d1b7b94"));
    const auto publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended));
    const auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "one1a50tun737ulcvwy0yvve0pvu5skq0kjargvhwe");

    const auto publicKey2 = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeED25519));
    EXPECT_ANY_THROW(new Address(publicKey2));
}

} // namespace TW::Harmony::tests
