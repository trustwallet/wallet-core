// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWAnyAddress.h>
#include "HexCoding.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(TWPivx, Address) {
    auto string = STRING("DSHhD6WxT5RfG2deUzVfuC1jDuRfdDpsSo");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypePivx));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "e7fae8ee6ecabaab1252f3b27679cb34f2406169");

    string = STRING("D6E4vLacYhKc6RVxLNQg5j8rtWbAH8fybH");
    addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypePivx));
    string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "0be8da968f9bc6c1c16b8c635544e757aade7013");
}
