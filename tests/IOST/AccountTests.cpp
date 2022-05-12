// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base58.h"
#include "IOST/Account.h"
#include <gtest/gtest.h>
using namespace TW;
using namespace TW::IOST;



TEST(IOSTAccount, Valid) {
    ASSERT_TRUE(Account::isValid("lispczz"));
    ASSERT_FALSE(Account::isValid("nil"));
    ASSERT_FALSE(Account::isValid("thisisaverylongstring"));
    ASSERT_FALSE(Account::isValid("invalid character"));
    ASSERT_FALSE(Account::isValid("BIGCASE"));
}


TEST(IOSTAccount, EncodePubKey) {
    auto fullKey = Base58::bitcoin.decode("2Gk8MxhWJQhsLXfUH87iQAPdVWsZ8aUXHcJAst4tzGYCGBaqE2VMABcCQYd6WbFm5PWDDxLNAr2yaGkHRCJnsSfo");
    auto key = PrivateKey(Data(fullKey.begin(), fullKey.begin() + 32));
    auto publicKey = key.getPublicKey(TWPublicKeyTypeED25519);
    auto encodePubKey=Account::encodePubKey(publicKey);
    ASSERT_EQ(encodePubKey,"2QNt4yU73VfwLyGdKj5yirtRJn5Zy7pB3KAGyUiTSfmP");
}