// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include <TrustWalletCore/TWAnyAddress.h>

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(TWOdinAnyAddress, IsValid) {
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("odin1fzjnsuwzxznqd3qlkt5njd4vvpcjqca6zckfux").get(),
                                    TWCoinTypeOdin));
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("odin10x79vuvw8adqfnvukda6j2vr8z238484nzazs7").get(),
                                    TWCoinTypeOdin));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02").get(),
                                     TWCoinTypeOdin));
}

TEST(TWOdinAnyAddress, Create) {
    auto string = STRING("odin1d224epchv57ftr6qwq98nzllfw9mmtzpfg4zla");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeOdin));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "6a955c8717653c958f40700a798bff4b8bbdac41");
}
