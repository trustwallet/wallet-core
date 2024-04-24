// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWAnyAddress.h>
#include "HexCoding.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(TWZen, Address) {
    auto string = STRING("znfexeyosWvMG93AjJx6CkRzKtS2aBdDgAx");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeZen));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "9fd1b64dad29d82b151206f66057bab1dae2f517");
}
