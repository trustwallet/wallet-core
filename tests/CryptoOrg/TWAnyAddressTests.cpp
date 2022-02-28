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

TEST(CryptoorgAnyAddress, IsValid) {
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("cro1ctwtcwpgksky988dhth6jslxveumgu0d45zgf0").get(), TWCoinTypeCryptoOrg));
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("cro1xpahy6c7wldxacv6ld99h435mhvfnsup24vcus").get(), TWCoinTypeCryptoOrg));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02").get(), TWCoinTypeCryptoOrg));
}

TEST(CryptoorgAnyAddress, Create) {
    auto string = STRING("cro1ctwtcwpgksky988dhth6jslxveumgu0d45zgf0");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeCryptoOrg));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "c2dcbc3828b42c429cedbaefa943e66679b471ed");
}
