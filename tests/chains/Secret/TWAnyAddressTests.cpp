// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyAddress.h>
#include "HexCoding.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(TWSecretAnyAddress, IsValid) {
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("secret16vw3fp7x35tzmwlkdkyzr8vgscn0zewtduyjuf").get(), TWCoinTypeSecret));
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("secret15rgv8teecnt53h0gdvngzt3am3yuz3rxh4fnle").get(), TWCoinTypeSecret));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02").get(), TWCoinTypeSecret));
}

TEST(TWSecretAnyAddress, Create) {
    auto string = STRING("secret16vw3fp7x35tzmwlkdkyzr8vgscn0zewtduyjuf");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeSecret));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "d31d1487c68d162dbbf66d88219d888626f165cb");
}
