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
    ASSERT_FALSE(Address::isValid("NULSd6HgbwcM8wz48f6UkFYHLVriT1L81X9z"));
}

TEST(NULSAddress, StaticIsValid) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_TRUE(Address::isValid("NULSd6HgUxmcJWc88iELEJ7RH9XHsazBQqnJc"));
    ASSERT_TRUE(Address::isValid("NULSd6HgbwcM8wz48f6UkFYHLVriT1L81X9z2"));
}

TEST(NULSAddress, IsValid) {
    ASSERT_FALSE(Address("NULSd6HgbwcM8wz48f6UkFYHLVriT1L81X9we").isValid());
    ASSERT_TRUE(Address("NULSd6HgbwcM8wz48f6UkFYHLVriT1L81X9z2").isValid());
}

TEST(NULSAddress, ChainID) {
    const auto address = Address("NULSd6HgbwcM8wz48f6UkFYHLVriT1L81X9z2");
    ASSERT_TRUE(address.chainID() == 1);
}

TEST(NULSAddress, Type) {
    const auto address = Address("NULSd6HgbwcM8wz48f6UkFYHLVriT1L81X9z2");
    ASSERT_TRUE(address.type() == 1);
}

TEST(NULSAddress, FromString) {
    const auto address = Address("NULSd6HgbwcM8wz48f6UkFYHLVriT1L81X9z2");
    ASSERT_EQ(address.string(), "NULSd6HgbwcM8wz48f6UkFYHLVriT1L81X9z2");
}

TEST(NULSAddress, FromData) {
    //const auto address = Address(parse_hex("0423017ed91a950f9734ee8062f2fcb9be3fe29aba07aef3"));
    //ASSERT_EQ(address.string(), "Nse1vKX9QHF7A84MHqrh4rRubVHAPc18");
}

TEST(NULSAddress, FromPrivateKey) {
    const auto privateKey =
        PrivateKey(parse_hex("a1269039e4ffdf43687852d7247a295f0b5bc55e6dda031cffaa3295ca0a9d7a"));
    const auto publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const auto address = Address(publicKey);

    ASSERT_EQ(address.string(), "NULSd6HghWa4CN5qdxqMwYVikQxRZyj57Jn4L");
}

TEST(NULSAddress, FromCompressedPublicKey) {
    const auto publicKey =
        PublicKey(parse_hex("0244d50ff36c3136b4bf81f0c74b066695bc2af43e28d7f0ca1d48fcfd084bea66"), TWPublicKeyTypeSECP256k1);
    const auto address = Address(publicKey);

    ASSERT_EQ(address.string(), "NULSd6HgUiMKPNi221bPfqvvho8QpuYBvn1x3");
}

TEST(NULSAddress, FromPrivateKey33) {
    const auto privateKey = NULS::Address::importHexPrivateKey(
       "d77580833f0b3c35b7114c23d6b66790d726c308baf237ec8c369152f2c08d27");
    const auto publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const auto address = Address(publicKey);

    ASSERT_EQ(address.string(), "NULSd6HgXx8YkwEjePLWUmdRSZzPQzK6BXnsB");
}
