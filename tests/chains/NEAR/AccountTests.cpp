// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "NEAR/Account.h"
#include "HexCoding.h"
#include <gtest/gtest.h>

namespace TW::NEAR::tests {

TEST(NEARAccount, Validation) {
    ASSERT_FALSE(Account::isValid("a"));
    ASSERT_FALSE(Account::isValid("!?:"));
    ASSERT_FALSE(Account::isValid("11111111111111111111111111111111222222222222222222222222222222223"));

    ASSERT_TRUE(Account::isValid("9902c136629fc630416e50d4f2fef6aff867ea7e.lockup.near"));
    ASSERT_TRUE(Account::isValid("app_1.alice.near"));
    ASSERT_TRUE(Account::isValid("test-trust.vlad.near"));
    ASSERT_TRUE(Account::isValid("deadbeef"));
}

} // namespace TW::NEAR::tests
