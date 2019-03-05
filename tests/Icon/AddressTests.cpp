// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Icon/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Icon;

TEST(IconAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_FALSE(Address::isValid("aaeb60f3e94c9b9a09f33669435e7ef1beaed"));
}

TEST(IconAddress, Valid) {
    ASSERT_TRUE(Address::isValid("hx116f042497e5f34268b1b91e742680f84cf4e9f3"));
    ASSERT_TRUE(Address::isValid("cx116f042497e5f34268b1b91e742680f84cf4e9f3"));
}

TEST(IconAddress, FromPrivateKey) {
    const auto privateKey = PrivateKey(parse_hex("94d1a980d5e528067d44bf8a60d646f556e40ca71e17cd4ead2d56f89e4bd20f"));
    const auto publicKey = PublicKey(privateKey.getPublicKey(false));
    const auto address = Address(publicKey, TWIconAddressTypeAddress);
    ASSERT_EQ(address.string(), "hx98c0832ca5bd8e8bf355ca9491888aa9725c2c48");
}

TEST(IconAddress, String) {
    const auto address = Address("hx116f042497e5f34268b1b91e742680f84cf4e9f3");
    ASSERT_EQ(address.string(), "hx116f042497e5f34268b1b91e742680f84cf4e9f3");
}
