// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "NEO/Address.h"
#include "NEO/Signer.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;

namespace TW::NEO::tests {

TEST(NEOAddress, FromPublicKey) {
    const auto publicKey = PublicKey(parse_hex("0222b2277d039d67f4197a638dd5a1d99c290b17aa8c4a16ccee5165fe612de66a"), TWPublicKeyTypeSECP256k1);
    const auto address = Address(publicKey);
    EXPECT_EQ(string("AKmrAHRD9ZDUnu4m3vWWonpsojo4vgSuqp"), address.string());
}

TEST(NEOAddress, FromString) {
    string neoAddress = "AXkgwcMJTy9wTAXHsbyhauxh7t2Tt31MmC";
    const auto address = Address(neoAddress);
    EXPECT_EQ(address.string(), neoAddress);
}

TEST(NEOAddress, isValid) {
    string neoAddress = "AQAsqiyHS4SSVWZ4CmMmnCxWg7vJ84GEj4";
    string bitcoinAddress = "1Ma2DrB78K7jmAwaomqZNRMCvgQrNjE2QC";

    EXPECT_TRUE(Address::isValid(neoAddress));
    EXPECT_FALSE(Address::isValid(bitcoinAddress));
}

TEST(NEOAddress, validation) {
    EXPECT_FALSE(Address::isValid("abc"));
    EXPECT_FALSE(Address::isValid("abeb60f3e94c1b9a09f33669435e7ef12eacd"));
    EXPECT_FALSE(Address::isValid("abcb60f3e94c9b9a09f33669435e7ef1beaedads"));
    EXPECT_TRUE(Address::isValid("ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD"));
}

TEST(NEOAddress, fromPubKey) {
    auto address = Address(PublicKey(parse_hex("031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486"), TWPublicKeyTypeNIST256p1));
    EXPECT_EQ("AeicEjZyiXKgUeSBbYQHxsU1X3V5Buori5", address.string());
}

TEST(NEOAddress, fromString) {
    auto b58Str = "AYTxeseHT5khTWhtWX1pFFP1mbQrd4q1zz";
    auto address = Address(b58Str);
    EXPECT_EQ(b58Str, address.string());
    auto errB58Str = "AATxeseHT5khTWhtWX1pFFP1mbQrd4q1zz";
    EXPECT_THROW(new Address(errB58Str), std::invalid_argument);
}

TEST(NEOAddress, Valid) {
    ASSERT_TRUE(Address::isValid("ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD"));
}

TEST(NEOAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("ANDfjwrUr54515515155WKRMyxFwvVwnZD"));
}

TEST(NEOAddress, FromPrivateKey) {
    auto key = PrivateKey(parse_hex("0x2A9EAB0FEC93CD94FA0A209AC5604602C1F0105FB02EAB398E17B4517C2FFBAB"));
    auto publicKey = key.getPublicKey(TWPublicKeyTypeNIST256p1);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "AQCSMB3oSDA1dHPn6GXN6KB4NHmdo1fX41");
}

} // namespace TW::NEO::tests
