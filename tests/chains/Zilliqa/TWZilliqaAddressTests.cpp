// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWAnyAddress.h>

#include <gtest/gtest.h>

TEST(TWZilliqa, Address) {

    auto string = STRING("zil1mk6pqphhkmaguhalq6n3cq0h38ltcehg0rfmv6");
    EXPECT_TRUE(TWAnyAddressIsValid(string.get(), TWCoinTypeZilliqa));

    EXPECT_FALSE(TWAnyAddressIsValid(STRING("0x7FCcaCf066a5F26Ee3AFfc2ED1FA9810Deaa632C").get(), TWCoinTypeZilliqa));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4").get(), TWCoinTypeZilliqa));
    EXPECT_FALSE(TWAnyAddressCreateWithString(STRING("bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4").get(), TWCoinTypeZilliqa) != NULL);

    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeZilliqa));
    auto desc = WRAPS(TWAnyAddressDescription(address.get()));

    auto expectedKeyHash = "Ddb41006F7B6FA8e5FBF06A71c01F789FeBC66e8";
    auto keyHash = WRAPD(TWAnyAddressData(address.get()));
    auto keyHashString = WRAPS(TWStringCreateWithRawBytes(TWDataBytes(keyHash.get()), strnlen(expectedKeyHash, 40)));

    assertStringsEqual(desc, "zil1mk6pqphhkmaguhalq6n3cq0h38ltcehg0rfmv6");
    assertStringsEqual(keyHashString, expectedKeyHash);
}
