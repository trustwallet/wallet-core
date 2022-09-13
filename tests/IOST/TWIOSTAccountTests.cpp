// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../interface/TWTestUtilities.h"

#include "Data.h"
#include <TrustWalletCore/TWIOSTAccount.h>

#include <gtest/gtest.h>

namespace TW::IOST::tests {

const char* validAddrStr1 = "Gcv8c2tH8qZrUYnKdEEdTtASsxivic2834MQW6mgxqto";

TEST(TWIOSTAccount, CreateAndDelete) {
    {
        TWIOSTAccount* addr = TWIOSTAccountCreateWithString(STRING(validAddrStr1).get());
        EXPECT_TRUE(addr != nullptr);
        TWIOSTAccountDelete(addr);
    }
}

TEST(TWIOSTAccount, IsValidString) {
    ASSERT_TRUE(TWIOSTAccountIsValidString(STRING(validAddrStr1).get()));
}

TEST(TWIOSTAccount, AddressDescription) {

    auto addr1 = WRAP(TWIOSTAccount, TWIOSTAccountCreateWithString(STRING(validAddrStr1).get()));
    EXPECT_TRUE(addr1.get() != nullptr);
    auto addrStr1 = std::string(TWStringUTF8Bytes(WRAPS(TWIOSTAccountDescription(addr1.get())).get()));
    EXPECT_TRUE(addrStr1 == validAddrStr1);
}

} // namespace TW::IOST::tests