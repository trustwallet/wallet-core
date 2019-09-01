// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWTelegramAddress.h>
#include <TrustWalletCore/TWString.h>

#include "TWTestUtilities.h"

#include <gtest/gtest.h>

TEST(TWTelegramAddress, AddressFromString) {
    auto addrStr = TWStringCreateWithUTF8Bytes("Ef+BVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODSkb");
    
    bool isValid = TWTelegramAddressIsValidString(addrStr);
    ASSERT_TRUE(isValid);

    // create address
    auto address = TWTelegramAddressCreateWithString(addrStr);
    // convert back to string
    auto str2 = TWTelegramAddressDescription(address);
    ASSERT_EQ(std::string(TWStringUTF8Bytes(addrStr)), std::string(TWStringUTF8Bytes(str2)) );
}
