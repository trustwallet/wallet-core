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

TEST(TWNebl, Address) {
    auto string = STRING("NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeNebl));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "ae40b2142aba5ddd10f74d9440bfda8a36cbad5b");
}
