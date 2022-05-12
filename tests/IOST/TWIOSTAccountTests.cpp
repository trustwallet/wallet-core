// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "proto/IOST.pb.h"
#include <TrustWalletCore/TWAnyAddress.h>
#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWIOSTAccount.h>
#include <gtest/gtest.h>
#include "Base58.h"

using namespace TW;
using namespace TW::IOST;

TEST(TWIOSTAccount, AccountCreateWithString) {
    auto account = TWIOSTAccountCreateWithString(STRING("testiost").get());
    auto description = WRAPS(TWIOSTAccountDescription(account));
    assertStringsEqual(description, "testiost");
    TWIOSTAccountDelete(account);
}

TEST(TWIOSTAccount, AccountEqual) {
    auto account1 = TWIOSTAccountCreateWithString(STRING("testiost").get());
    auto account2 = TWIOSTAccountCreateWithString(STRING("testiost").get());
    auto account3 = TWIOSTAccountCreateWithString(STRING("testiostiost").get());
    ASSERT_TRUE(TWIOSTAccountEqual(account1,account2));
    ASSERT_FALSE(TWIOSTAccountEqual(account1,account3));

    TWIOSTAccountDelete(account1);
    TWIOSTAccountDelete(account2);
    TWIOSTAccountDelete(account3);
}

TEST(TWIOSTAccount, AccountIsValidString) {
    ASSERT_TRUE(TWIOSTAccountIsValidString(STRING("lispczz").get()));
    ASSERT_FALSE(TWIOSTAccountIsValidString(STRING("nil").get()));
    ASSERT_FALSE(TWIOSTAccountIsValidString(STRING("thisisaverylongstring").get()));
    ASSERT_FALSE(TWIOSTAccountIsValidString(STRING("invalid character").get()));
    ASSERT_FALSE(TWIOSTAccountIsValidString(STRING("BIGCASE").get()));
}