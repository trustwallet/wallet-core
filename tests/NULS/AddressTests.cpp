// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#include "NULS/Address.h"

#include "HexCoding.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::NULS;

/*
 *
 {
    "address": "NsdzroTToPjWGSqz9ZpWJNSvDRhtcE7S",
    "encryptedPrivateKey": null,
    "alias": null,
    "pubKey": "03d38bb716c3a641895ffaa480c29cc7aa5f75ed550138a52e644e94afc9d0cd03",
    "prikey": "00e4e0454476a38d9c5dbf332cf576f6dd77c6941e41c64be3ae22b334f3bed2de"
 }
 *
 * */

TEST(NULSAddress, StaticInvalid) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_FALSE(Address::isValid("aaeb60f3e94c9b9a09f33669435e7ef1beaed"));
    ASSERT_FALSE(Address::isValid("Nse3uLgeCBWP48GCGh8L54gnELfpnSG8"));
}

TEST(NULSAddress, StaticIsValid) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_TRUE(Address::isValid("Nse3uLgeCBWP48GCGh8L54gnELfpnSG9"));
    ASSERT_TRUE(Address::isValid("Nse1vKX9QHF7A84MHqrh4rRubVHAPc18"));
}

TEST(NULSAddress, IsValid) {
    ASSERT_FALSE(Address("Nse3uLgeCBWP48GCGx8L54gnELfpnSG9").isValid());
    ASSERT_TRUE(Address("NsdzroTToPjWGSqz9ZpWJNSvDRhtcE7S").isValid());
}

TEST(NULSAddress, ChainID) {
    const auto address = Address("Nse1vKX9QHF7A84MHqrh4rRubVHAPc18");
    ASSERT_TRUE(address.chainID() == 8964);
}

TEST(NULSAddress, Type) {
    const auto address = Address("Nse1vKX9QHF7A84MHqrh4rRubVHAPc18");
    ASSERT_TRUE(address.type() == 1);
}

TEST(NULSAddress, FromString) {
    const auto address = Address("Nse1vKX9QHF7A84MHqrh4rRubVHAPc18");
    ASSERT_EQ(address.string(), "Nse1vKX9QHF7A84MHqrh4rRubVHAPc18");
}

TEST(NULSAddress, FromData) {
    const auto address = Address(parse_hex("0423017ed91a950f9734ee8062f2fcb9be3fe29aba07aef3"));
    ASSERT_EQ(address.string(), "Nse1vKX9QHF7A84MHqrh4rRubVHAPc18");
}

TEST(NULSAddress, FromPrivateKey) {
    const auto privateKey =
        PrivateKey(parse_hex("e4e0454476a38d9c5dbf332cf576f6dd77c6941e41c64be3ae22b334f3bed2de"));
    const auto publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const auto address = Address(publicKey);

    ASSERT_EQ(address.string(), "NsdzroTToPjWGSqz9ZpWJNSvDRhtcE7S");
}

TEST(NULSAddress, FromCompressedPublicKey) {
    const auto publicKey =
        PublicKey(parse_hex("03d38bb716c3a641895ffaa480c29cc7aa5f75ed550138a52e644e94afc9d0cd03"), TWPublicKeyTypeSECP256k1);
    const auto address = Address(publicKey);

    ASSERT_EQ(address.string(), "NsdzroTToPjWGSqz9ZpWJNSvDRhtcE7S");
}

TEST(NULSAddress, FromPrivateKey33) {
    const auto privateKey = NULS::Address::importHexPrivateKey(
        "00a2139f4972f19f82fa26f08166880913be795bc6e1d596c2108fc4f60f2ec5f2");
    const auto publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const auto address = Address(publicKey);

    ASSERT_EQ(address.string(), "NsdvEERpbh9PyhVGBuE4Tgkee83HYbdj");
}
