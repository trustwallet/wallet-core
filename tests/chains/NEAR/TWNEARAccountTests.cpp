// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
//

#include "TestUtilities.h"
#include <TrustWalletCore/TWNEARAccount.h>
#include <gtest/gtest.h>


TEST(TWNEARAccount, description) {
    {
        auto string = STRING("NEARTDDWrUMdoC2rA1eU6gNrSU2zyGKdR71TNucTvsQHyfAXjKcJb");
        auto account = TWNEARAccountCreateWithString(string.get());
        auto description = WRAPS(TWNEARAccountDescription(account));
        assertStringsEqual(description, "3b83b07cab54824a59c3d3f2e203a7cd913b7fcdc4439595983e2402c2cf791d");
        TWNEARAccountDelete(account);
    }

    {
        auto account = TWNEARAccountCreateWithString(STRING("test.near").get());
        auto description = WRAPS(TWNEARAccountDescription(account));

        assertStringsEqual(description, "test.near");

        TWNEARAccountDelete(account);
    }

    {
        auto string = STRING("grate!.near");
        auto account = TWNEARAccountCreateWithString(string.get());

        ASSERT_EQ(account, nullptr);
    }
}
