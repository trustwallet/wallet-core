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

TEST(TWBitcoinDiamond, Address) {
    auto string = STRING("3CDf39adX4mc1AnvDzYHjw2NhxKswAPV3y");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeBitcoinDiamond));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "737cb7c194ec6502be59ed985d66b8bfe8b2b986");

    string = STRING("1DH9cvKqGgzCvwoap45Nh75qV62wqje9pJ");
    addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeBitcoinDiamond));
    string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "86af5c1d5e754fc8906ec3c5d26e0135e1cb7c85");

    // segwit addrerss
    string = STRING("bcd1q35t4g0ezx9cl5qzac4tl377n9492uztqjwf5wh");
    addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeBitcoinDiamond));
    string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "8d17543f223171fa005dc557f8fbd32d4aae0960");
}
