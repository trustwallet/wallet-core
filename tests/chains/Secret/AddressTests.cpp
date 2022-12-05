// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Cosmos/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

namespace TW::Cosmos::tests {

TEST(SecretAddress, Valid) {
    ASSERT_TRUE(Address::isValid(TWCoinTypeSecret, "secret16vw3fp7x35tzmwlkdkyzr8vgscn0zewtduyjuf"));
    ASSERT_TRUE(Address::isValid(TWCoinTypeSecret, "secret15rgv8teecnt53h0gdvngzt3am3yuz3rxh4fnle"));
    ASSERT_TRUE(Address::isValid(TWCoinTypeSecret, "secret1pjp5dpvrjumn653dycszk3g264zgqusz2xhdq7"));
}

TEST(SecretAddress, Invalid) {
    ASSERT_FALSE(Address::isValid(TWCoinTypeSecret, "secret15rgv8teecnt53h0gdvngzt3am3yuz3rxh4fnla"));
    ASSERT_FALSE(Address::isValid(TWCoinTypeSecret, "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02")); // valid cosmos
}

TEST(SecretAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("92ded9fb8b4646b66d0c127fa38523133ce11bdc5dadee2d5fc1c0ccd9fec664"));
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Address(TWCoinTypeSecret, publicKey);
    ASSERT_EQ(address.string(), "secret16vw3fp7x35tzmwlkdkyzr8vgscn0zewtduyjuf");
}

TEST(SecretAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("0250bba49f3d5a574b38de891387420877de10fd44de609567d5e6e0c97bfa4bb2"), TWPublicKeyTypeSECP256k1);
    auto address = Address(TWCoinTypeSecret, publicKey);
    ASSERT_EQ(address.string(), "secret16vw3fp7x35tzmwlkdkyzr8vgscn0zewtduyjuf");
}

TEST(SecretAddress, FromString) {
    Address address;
    EXPECT_TRUE(Address::decode("secret16vw3fp7x35tzmwlkdkyzr8vgscn0zewtduyjuf", address));
    EXPECT_EQ(address.string(), "secret16vw3fp7x35tzmwlkdkyzr8vgscn0zewtduyjuf");
    EXPECT_EQ(hex(address.getKeyHash()), "d31d1487c68d162dbbf66d88219d888626f165cb");
}

} // namespace TW::Cosmos::tests
