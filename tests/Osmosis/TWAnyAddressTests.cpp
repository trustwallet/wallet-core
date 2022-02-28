// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyAddress.h>
#include "HexCoding.h"

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(TWOsmosisAnyAddress, IsValid) {
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("osmo1mky69cn8ektwy0845vec9upsdphktxt0en97f5").get(), TWCoinTypeOsmosis));
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("osmo18s0hdnsllgcclweu9aymw4ngktr2k0rkvn7jmn").get(), TWCoinTypeOsmosis));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02").get(), TWCoinTypeOsmosis));
}

TEST(TWOsmosisAnyAddress, Create) {
    auto string = STRING("osmo1mky69cn8ektwy0845vec9upsdphktxt0en97f5");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeOsmosis));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "dd89a2e267cd96e23cf5a33382f030686f65996f");
}
