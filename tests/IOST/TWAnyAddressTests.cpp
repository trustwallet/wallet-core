// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base58.h"
#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWIOSTAccount.h>
#include <gtest/gtest.h>

TEST(TWIOSTAnyAddress, Address) {

    auto string = STRING("2QNt4yU73VfwLyGdKj5yirtRJn5Zy7pB3KAGyUiTSfmP");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeIOST));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));

    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "14d8c92fceefdc967f1f6cbace65a9f0740bfe45742a478cc0c04cca341cfb4e");
}