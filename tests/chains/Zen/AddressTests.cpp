// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "HDWallet.h"
#include "Zen/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Zen;

TEST(ZenAddress, Valid) {
    ASSERT_TRUE(Address::isValid("znk19H1wcARcCa7TM6zgmJUbWoWWtZ8k5cg"));
    ASSERT_TRUE(Address::isValid("zshX5BAgUvNgM1VoBVKZyFVVozTDjjJvRxJ"));
}

TEST(ZenAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("znk19H1wcARcCa7TM6zgmJUbWoWWtZ8k5abs"));
}

TEST(ZenAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("3a8e0a528f62f4ca2c77744c8a571def2845079b50105a9f7ef6b1b823def67a"));
    auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Address(pubKey);
    ASSERT_EQ(address.string(), "znk19H1wcARcCa7TM6zgmJUbWoWWtZ8k5cg");
}

TEST(ZenAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("02b4ac9056d20c52ac11b0d7e83715dd3eac851cfc9cb64b8546d9ea0d4bb3bdfe"), TWPublicKeyTypeSECP256k1);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "znk19H1wcARcCa7TM6zgmJUbWoWWtZ8k5cg");
}

TEST(ZenAddress, FromString) {
    auto address = Address("znk19H1wcARcCa7TM6zgmJUbWoWWtZ8k5cg");
    ASSERT_EQ(address.string(), "znk19H1wcARcCa7TM6zgmJUbWoWWtZ8k5cg");

    address = Address("zshX5BAgUvNgM1VoBVKZyFVVozTDjjJvRxJ");
    ASSERT_EQ(address.string(), "zshX5BAgUvNgM1VoBVKZyFVVozTDjjJvRxJ");
}
