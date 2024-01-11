// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "IOST/Account.h"
#include "proto/IOST.pb.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::IOST;

TEST(IOSTAddress, ValidateAccount) {
    // https://www.iostabc.com/tx/DnR4QuRDJAjUZ2qfJK9MT92p95BBub2FnyigeXn2Z1es
    ASSERT_TRUE(Account::isValid("12345xusong"));
    ASSERT_TRUE(Account::isValid("12345"));
    ASSERT_TRUE(Account::isValid("1234_xuson"));
    ASSERT_TRUE(Account::isValid("EKRQPgX7nKt8hJABwm9m3BKWGj7kcSECkJnCBauHQWin"));

    ASSERT_FALSE(TW::validateAddress(TWCoinTypeIOST, "1234"));
    ASSERT_FALSE(TW::validateAddress(TWCoinTypeIOST, "1234_xusonG"));
    ASSERT_FALSE(TW::validateAddress(TWCoinTypeIOST, "12345xusong6"));
    ASSERT_FALSE(TW::validateAddress(TWCoinTypeIOST, "bc1quvuarfksewfeuevuc6tn0kfyptgjvwsvrprk9d"));
    ASSERT_FALSE(TW::validateAddress(TWCoinTypeIOST, "DJRFZNg8jkUtjcpo2zJd92FUAzwRjitw6f"));

    ASSERT_EQ(TW::normalizeAddress(TWCoinTypeIOST, "EKRQPgX7nKt8hJABwm9m3BKWGj7kcSECkJnCBauHQWin"), "EKRQPgX7nKt8hJABwm9m3BKWGj7kcSECkJnCBauHQWin");
    ASSERT_EQ(TW::normalizeAddress(TWCoinTypeIOST, "Gcv8c2tH8qZrUYnKdEEdTtASsxivic2834MQW6mgxqto"), "Gcv8c2tH8qZrUYnKdEEdTtASsxivic2834MQW6mgxqto");
}

TEST(IOSTAddress, Account) {
    std::string key = ("63095105a37b4e896e5ebbd740e751c6f9df7cca2410beba3261dc5680299cebe"
                       "812b52ea9ad5cba9a9af03afcc6f2942a4524b0df3c0344dc195072831670c4");
    Data secKeyBytes = parse_hex(key);
    std::string secKey(secKeyBytes.begin(), secKeyBytes.end());

    Proto::AccountInfo ai;
    ai.set_active_key(secKey);
    ai.set_owner_key(secKey);

    Account account(ai);
    ASSERT_EQ(hex(account.activeKey), "63095105a37b4e896e5ebbd740e751c6f9df7cca2410beba3261dc5680299ceb");
    EXPECT_EQ(hex(account.ownerKey), "63095105a37b4e896e5ebbd740e751c6f9df7cca2410beba3261dc5680299ceb");

    auto pubKey = account.publicOwnerKey();
    auto address = Account::address(std::string(pubKey.begin(), pubKey.end()));
    EXPECT_EQ(address, "Gcv8c2tH8qZrUYnKdEEdTtASsxivic2834MQW6mgxqto");

    EXPECT_EQ(hex(TW::addressToData(TWCoinTypeIOST, address)), "e812b52ea9ad5cba9a9af03afcc6f2942a4524b0df3c0344dc195072831670c4");
}

TEST(IOSTAddress, FromPrivateKey) {
    auto wallet = HDWallet(
        "shoot island position soft burden budget tooth cruel issue economy destroy above", "");
    auto address = wallet.deriveAddress(TWCoinTypeIOST);
    ASSERT_EQ(address, "4av8w81EyzUgHonsVWqfs15WM4Vrpgox4BYYQWhNQDVu");
}

TEST(IOSTAddress, EqualOperator) {
    auto acnt1 = Account("account1");
    auto acnt2 = Account("account2");
    ASSERT_TRUE(acnt1 == acnt1);
    ASSERT_FALSE(acnt1 == acnt2);
}