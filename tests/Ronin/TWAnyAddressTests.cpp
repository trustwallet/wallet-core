// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../interface/TWTestUtilities.h"

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>

TEST(RoninAnyAddress, Validate) {
    auto ethereum = STRING("0xEC49280228b0D05Aa8e8b756503254e1eE7835ab");
    EXPECT_FALSE(TWAnyAddressIsValid(ethereum.get(), TWCoinTypeRonin));

    auto string = STRING("ronin:ec49280228b0d05aa8e8b756503254e1ee7835ab");
    auto checksummed = STRING("ronin:EC49280228b0D05Aa8e8b756503254e1eE7835ab");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(checksummed.get(), TWCoinTypeRonin));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "ec49280228b0d05aa8e8b756503254e1ee7835ab");    
}
