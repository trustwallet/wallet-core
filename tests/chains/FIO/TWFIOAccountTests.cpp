// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"
#include <TrustWalletCore/TWFIOAccount.h>

#include <gtest/gtest.h>

TEST(TWFIO, Account) {
    {
        auto string = STRING("FIO6XKiobBGrmPZDaHne47TF25CamjFc3cRe8hs7oAiCN59TbJzes");
        auto account = TWFIOAccountCreateWithString(string.get());
        auto description = WRAPS(TWFIOAccountDescription(account));
        assertStringsEqual(description, "rpxtnpsr3gxd");
        TWFIOAccountDelete(account);
    }

    {
        auto account = TWFIOAccountCreateWithString(STRING("lhp1ytjibtea").get());
        auto account2 = TWFIOAccountCreateWithString(STRING("wrcjejslfplp").get());
        auto account3 = TWFIOAccountCreateWithString(STRING("rewards@wallet").get());
        auto description = WRAPS(TWFIOAccountDescription(account));
        auto description2 = WRAPS(TWFIOAccountDescription(account2));
        auto description3 = WRAPS(TWFIOAccountDescription(account3));
        assertStringsEqual(description, "lhp1ytjibtea");
        assertStringsEqual(description2, "wrcjejslfplp");
        assertStringsEqual(description3, "rewards@wallet");

        TWFIOAccountDelete(account);
        TWFIOAccountDelete(account2);
        TWFIOAccountDelete(account3);
    }

    {
        auto string = STRING("asdf19s");
        auto string2 = STRING("0x320196ef1b137786be51aa391e78e0a2c756f46b");
        auto account = TWFIOAccountCreateWithString(string.get());
        auto account2 = TWFIOAccountCreateWithString(string2.get());

        ASSERT_EQ(account, nullptr);
        ASSERT_EQ(account2, nullptr);
    }
}
