// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ripple/Address.h"
#include "Bitcoin/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::Ripple;

TEST(RippleAddress, FromPublicKey) {
    const auto publicKey = PublicKey(parse_hex("0303E20EC6B4A39A629815AE02C0A1393B9225E3B890CAE45B59F42FA29BE9668D"));
    const auto address = Address(publicKey);
    auto str = hex(address.bytes);
    ASSERT_EQ(string("rnBFvgZphmN39GWzUJeUitaP22Fr9be75H"), address.string());
}

TEST(RippleAddress, FromString) {
    string rippleAddress = "rnBFvgZphmN39GWzUJeUitaP22Fr9be75H";
    const auto address = Address(rippleAddress);

    ASSERT_EQ(address.string(), rippleAddress);
}

TEST(RippleAddress, isValid) {
    string rippleAddress = "r36yxStAh7qgTQNHTzjZvXybCTzUFhrfav";
    string bitcoinAddress = "1Ma2DrB78K7jmAwaomqZNRMCvgQrNjE2QC";

    ASSERT_TRUE(Address::isValid(rippleAddress));
    ASSERT_FALSE(Address::isValid(bitcoinAddress));
}
