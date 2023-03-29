// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>

TEST(CronosAnyAddress, Validate) {
    auto string = STRING("0xEC49280228b0D05Aa8e8b756503254e1eE7835ab");

    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeCronosChain));
    
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "ec49280228b0d05aa8e8b756503254e1ee7835ab");    
}
