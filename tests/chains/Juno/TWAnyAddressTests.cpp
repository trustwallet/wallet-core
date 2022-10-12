// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyAddress.h>
#include "HexCoding.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(TWJunoAnyAddress, IsValid) {
    EXPECT_TRUE(TWAnyAddressIsValidWithHrp(STRING("juno1gckvjxau7k56f8wg8c8xj80khyp83y8x8eqc94").get(), TWCoinTypeCosmos, "juno"));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("juno1gckvjxau7k56f8wg8c8xj80khyp83y8x8eqc94").get(), TWCoinTypeCosmos));
}
