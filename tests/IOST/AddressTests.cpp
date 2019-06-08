// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "IOST/Account.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::IOST;

TEST(IOSTAddress, ValidateAccount) {
    // https://www.iostabc.com/tx/DnR4QuRDJAjUZ2qfJK9MT92p95BBub2FnyigeXn2Z1es
    ASSERT_TRUE(Account::isValid("12345xusong"));
    ASSERT_TRUE(Account::isValid("12345"));
    ASSERT_TRUE(Account::isValid("1234_xuson"));
    ASSERT_TRUE(Account::isValid("EKRQPgX7nKt8hJABwm9m3BKWGj7kcSECkJnCBauHQWin"));

    ASSERT_FALSE(Account::isValid("1234"));
    ASSERT_FALSE(Account::isValid("1234_xusonG"));
    ASSERT_FALSE(Account::isValid("12345xusong6"));
    ASSERT_FALSE(Account::isValid("bc1quvuarfksewfeuevuc6tn0kfyptgjvwsvrprk9d"));
    ASSERT_FALSE(Account::isValid("DJRFZNg8jkUtjcpo2zJd92FUAzwRjitw6f"));
}
