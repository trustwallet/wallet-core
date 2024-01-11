// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include <TrustWalletCore/TWAnyAddress.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(TWInternetComputer, Address) {
    auto string = STRING("58b26ace22a36a0011608a130e84c7cf34ba469c38d24ccf606152ce7de91f4e");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeInternetComputer));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
}
