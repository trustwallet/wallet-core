// Copyright © 2017-2020 Trust Wallet.
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


TEST(NULSAddress, StaticInvalid) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_FALSE(Address::isValid("aaeb60f3e94c9b9a09f33669435e7ef1beaed"));
    ASSERT_FALSE(Address::isValid("NULSd6HgbwcM8wz48f6UkFYHLVriT1L81X9z"));
    ASSERT_TRUE(Address::isValid("NULSd6HgUxmcJWc88iELEJ7RH9XHsazBQqnJc"));
    ASSERT_TRUE(Address::isValid("NULSd6HgbwcM8wz48f6UkFYHLVriT1L81X9z2"));    
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
    const auto privateKey = PrivateKey(parse_hex("d77580833f0b3c35b7114c23d6b66790d726c308baf237ec8c369152f2c08d27"));
    const auto publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const auto address = Address(publicKey);

    ASSERT_EQ(address.string(), "NULSd6HgXx8YkwEjePLWUmdRSZzPQzK6BXnsB");
}
