// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ripple/Address.h"
#include "Ripple/XAddress.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::Ripple;

TEST(RippleAddress, FromPublicKey) {
    const auto publicKey = PublicKey(parse_hex("0303E20EC6B4A39A629815AE02C0A1393B9225E3B890CAE45B59F42FA29BE9668D"), TWPublicKeyTypeSECP256k1);
    const auto address = Address(publicKey);
    ASSERT_EQ(string("rnBFvgZphmN39GWzUJeUitaP22Fr9be75H"), address.string());
}

TEST(RippleAddress, FromString) {
    string classic = "rnBFvgZphmN39GWzUJeUitaP22Fr9be75H";
    const auto address = Address(classic);

    ASSERT_EQ(address.string(), classic);
}

TEST(RippleXAddress, FromPublicKey) {
    const auto publicKey = PublicKey(parse_hex("0303E20EC6B4A39A629815AE02C0A1393B9225E3B890CAE45B59F42FA29BE9668D"), TWPublicKeyTypeSECP256k1);
    const auto address = XAddress(publicKey, 12345);
    ASSERT_EQ(string("X76UnYEMbQfEs3mUqgtjp4zFy9exgThRj7XVZ6UxsdrBptF"), address.string());
}

TEST(RippleXAddress, FromString) {
    string xAddress = "X76UnYEMbQfEs3mUqgtjp4zFy9exgThRj7XVZ6UxsdrBptF";
    string xAddress2 = "X76UnYEMbQfEs3mUqgtjp4zFy9exgTsM93nriVZAPufrpE3";
    const auto address = XAddress(xAddress);
    const auto address2 = XAddress(xAddress2);

    ASSERT_EQ(address.tag, 12345);
    ASSERT_EQ(address.string(), xAddress);

    ASSERT_EQ(address2.tag, 0);
    ASSERT_EQ(address2.string(), xAddress2);
}

TEST(RippleAddress, isValid) {
    string classicAddress = "r36yxStAh7qgTQNHTzjZvXybCTzUFhrfav";
    string bitcoinAddress = "1Ma2DrB78K7jmAwaomqZNRMCvgQrNjE2QC";
    string xAddress = "XVfvixWZQKkcenFRYApCjpTUyJ4BePTe3jJv7beatUZvQYh";

    ASSERT_TRUE(Address::isValid(classicAddress));
    ASSERT_TRUE(XAddress::isValid(xAddress));
    ASSERT_FALSE(Address::isValid(bitcoinAddress));
    ASSERT_FALSE(XAddress::isValid(bitcoinAddress));
}
