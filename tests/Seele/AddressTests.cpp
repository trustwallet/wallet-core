// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Seele/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Seele;

TEST(SeeleAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_FALSE(Address::isValid("aaeb60f3e94c9b9a09f33669435e7ef1beaed"));
    ASSERT_TRUE(Address::isValid("0x416159759b9a305f27db4ef6a05f0f950dd4df21"));
}


TEST(SeeleAddress, String) {
    const auto address = Address("0x5bb497e8d9fe26e92dd1be01e32076c8e024d167");
    ASSERT_EQ(address.string(), "0x5bb497e8d9fe26e92dd1be01e32076c8e024d167");
}

TEST(SeeleAddress, FromPrivateKey) {
    const auto privateKey = PrivateKey(parse_hex("0xb66634d83e51c6dfc27c74efc74e70712d9f53d057984ab9a2395275bec746fd"));
    const auto publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended));
    const auto address = Address(publicKey);

    //ASSERT_EQ(hex(publicKey.bytes), "04cf53df46c1c483baba9e3cce56e76b96d5f5ab6ad200a3c4e3a3cb1f687fb7863909bfb44d7e6cb9246bdd7935aff104f9ec61beccec91033ceacf6a26262b09");

    ASSERT_EQ(address.string(), "0x416159759b9a305f27db4ef6a05f0f950dd4df21");
}

TEST(SeeleAddress, IsValid) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_TRUE(Address::isValid("0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"));
}
