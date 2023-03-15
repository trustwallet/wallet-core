// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "Zilliqa/Address.h"
#include "Zilliqa/AddressChecksum.h"

#include <TrustWalletCore/TWHRP.h>

#include <gtest/gtest.h>

namespace TW::Zilliqa::tests {

TEST(ZilliqaAddress, FromPrivateKey) {
    const auto privateKey =
        PrivateKey(parse_hex("3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6"));
    const auto publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const auto address = Address(publicKey);
    auto expectedAddress = "zil1j8xae6lggm8y63m3y2r7aefu797ze7mhzulnqg";

    ASSERT_EQ(address.getHrp(), stringForHRP(TWHRPZilliqa));
    ASSERT_EQ(address.string(), expectedAddress);
}

TEST(ZilliqaAddress, Validation) {
    ASSERT_FALSE(Zilliqa::Address::isValid("0x91cddcebe846ce4d47712287eee53cf17c2cfb7"));
    ASSERT_FALSE(Zilliqa::Address::isValid(""));
    ASSERT_FALSE(Zilliqa::Address::isValid("0x"));
    ASSERT_FALSE(Zilliqa::Address::isValid("91cddcebe846ce4d47712287eee53cf17c2cfb7"));

    ASSERT_TRUE(Zilliqa::Address::isValid("zil1fwh4ltdguhde9s7nysnp33d5wye6uqpugufkz7"));
}

TEST(ZilliqaAddress, Checksum) {
    ASSERT_EQ(
        checksum(parse_hex("4BAF5FADA8E5DB92C3D3242618C5B47133AE003C")),
        "4BAF5faDA8e5Db92C3d3242618c5B47133AE003C");
    ASSERT_EQ(
        checksum(parse_hex("448261915A80CDE9BDE7C7A791685200D3A0BF4E")),
        "448261915a80cdE9BDE7C7a791685200D3A0bf4E");
    ASSERT_EQ(
        checksum(parse_hex("0xDED02FD979FC2E55C0243BD2F52DF022C40ADA1E")),
        "Ded02fD979fC2e55c0243bd2F52df022c40ADa1E");
    ASSERT_EQ(
        checksum(parse_hex("0x13F06E60297BEA6A3C402F6F64C416A6B31E586E")),
        "13F06E60297bea6A3c402F6f64c416A6b31e586e");
    ASSERT_EQ(
        checksum(parse_hex("0x1A90C25307C3CC71958A83FA213A2362D859CF33")),
        "1a90C25307C3Cc71958A83fa213A2362D859CF33");
}

} // namespace TW::Zilliqa::tests
