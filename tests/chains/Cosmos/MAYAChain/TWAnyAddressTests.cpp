// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyAddress.h>
#include "HexCoding.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(MAYAChainAnyAddress, IsValid) {
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("maya1uuds8pd92qnnq0udw0rpg0szpgcslc9prlsqk8").get(), TWCoinTypeMAYAChain));
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("maya1zf3gsk7edzwl9syyefvfhle37cjtql35n6emmw").get(), TWCoinTypeMAYAChain));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02").get(), TWCoinTypeMAYAChain));
}

TEST(MAYAChainAnyAddress, Create) {
    auto string = STRING("maya1uuds8pd92qnnq0udw0rpg0szpgcslc9prlsqk8");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeMAYAChain));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "e71b0385a55027303f8d73c6143e020a310fe0a1");
}
