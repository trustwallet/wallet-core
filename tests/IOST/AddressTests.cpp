// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base58.h"
#include "IOST/Address.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include <gtest/gtest.h>
#include <vector>
using namespace TW;
using namespace TW::IOST;
using namespace std;

TEST(IOSTAddress, Valid) {
    ASSERT_TRUE(Address::isValid("2QNt4yU73VfwLyGdKj5yirtRJn5Zy7pB3KAGyUiTSfmP"));
}

TEST(IOSTAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("aaaa"));
    ASSERT_FALSE(Address::isValid("11112QNt4yU73VfwLyGdKj5yirtRJn5Zy7pB3KAGyUiTSfmP"));
}

TEST(IOSTAddress, FromPrivateKey) {
    auto fullKey = Base58::bitcoin.decode("2Gk8MxhWJQhsLXfUH87iQAPdVWsZ8aUXHcJAst4tzGYCGBaqE2VMABcCQYd6WbFm5PWDDxLNAr2yaGkHRCJnsSfo");
    auto key = PrivateKey(Data(fullKey.begin(), fullKey.begin() + 32));
    auto publicKey = key.getPublicKey(TWPublicKeyTypeED25519);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "2QNt4yU73VfwLyGdKj5yirtRJn5Zy7pB3KAGyUiTSfmP");
}

TEST(IOSTAddress, FromString) {
    auto address = Address("2QNt4yU73VfwLyGdKj5yirtRJn5Zy7pB3KAGyUiTSfmP");
    ASSERT_EQ(address.string(), "2QNt4yU73VfwLyGdKj5yirtRJn5Zy7pB3KAGyUiTSfmP");
}