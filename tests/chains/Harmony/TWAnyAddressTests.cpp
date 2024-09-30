// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>

TEST(HarmonyAnyAddress, Harmony) {
    auto string = STRING("one1c8dpswxg2p50znzecnq0peuxlxtcm9je7q7yje");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeHarmony));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));

    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));

    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "c1da1838c85068f14c59c4c0f0e786f9978d9659");
}
