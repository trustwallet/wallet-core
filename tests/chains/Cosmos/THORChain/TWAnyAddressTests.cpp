// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWAnyAddress.h>
#include "HexCoding.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(THORChainAnyAddress, IsValid) {
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r").get(), TWCoinTypeTHORChain));
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("thor1c8jd7ad9pcw4k3wkuqlkz4auv95mldr2kyhc65").get(), TWCoinTypeTHORChain));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02").get(), TWCoinTypeTHORChain));
}

TEST(THORChainAnyAddress, Create) {
    auto string = STRING("thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeTHORChain));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "1522e767db6eb19708b0038029bfbd607bc9bd0e");
}
