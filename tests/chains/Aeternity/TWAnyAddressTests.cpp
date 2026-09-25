// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWAnyAddress.h>
#include <gtest/gtest.h>
#include <TestUtilities.h>

namespace TW::Aeternity::tests {

// `TWAnyAddressIsValid` must catch exceptions and return false.
TEST(AeternityAddress, IsValid) {
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("invalid").get(), TWCoinTypeAeternity));
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("behave@wallet").get(), TWCoinTypeAeternity));
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("a").get(), TWCoinTypeAeternity));
}

} // namespace TW::Aeternity::tests
