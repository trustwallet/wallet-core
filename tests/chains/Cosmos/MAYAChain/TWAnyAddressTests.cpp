// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWAnyAddress.h>
#include "HexCoding.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(MAYAChainAnyAddress, IsValid) {
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("maya1z53wwe7md6cewz9sqwqzn0aavpaun0gw0wclun").get(), TWCoinTypeMAYAChain));
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("maya1c8jd7ad9pcw4k3wkuqlkz4auv95mldr2knf5vy").get(), TWCoinTypeMAYAChain));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02").get(), TWCoinTypeMAYAChain));
}

TEST(MAYAChainAnyAddress, Create) {
    auto string = STRING("maya1z53wwe7md6cewz9sqwqzn0aavpaun0gw0wclun");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeMAYAChain));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "1522e767db6eb19708b0038029bfbd607bc9bd0e");
}
