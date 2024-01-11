// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWAccount.h>

#include <gtest/gtest.h>

TEST(TWAccount, Create) {
    const auto addressAdd = "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny";
    const auto coin = TWCoinTypeBitcoin;
    const auto derivationPath = "m/84'/0'/0'/0/0";
    const auto extPubKeyAdd = "zpub6qbsWdbcKW9sC6shTKK4VEhfWvDCoWpfLnnVfYKHLHt31wKYUwH3aFDz4WLjZvjHZ5W4qVEyk37cRwzTbfrrT1Gnu8SgXawASnkdQ994atn";
    const auto pubKey = "02df6fc590ab3101bbe0bb5765cbaeab9b5dcfe09ac9315d707047cbd13bc7e006";

    const auto account = WRAP(
        TWAccount,
        TWAccountCreate(
            WRAPS(TWStringCreateWithUTF8Bytes(addressAdd)).get(),
            coin,
            TWDerivationDefault,
            WRAPS(TWStringCreateWithUTF8Bytes(derivationPath)).get(),
            WRAPS(TWStringCreateWithUTF8Bytes(pubKey)).get(),
            WRAPS(TWStringCreateWithUTF8Bytes(extPubKeyAdd)).get()
            )
        );

    assertStringsEqual(WRAPS(TWAccountAddress(account.get())), addressAdd);
    EXPECT_EQ(coin, TWAccountCoin(account.get()));
    EXPECT_EQ(TWAccountDerivation(account.get()), TWDerivationDefault);
    assertStringsEqual(WRAPS(TWAccountDerivationPath(account.get())), derivationPath);
    assertStringsEqual(WRAPS(TWAccountExtendedPublicKey(account.get())), extPubKeyAdd);
    assertStringsEqual(WRAPS(TWAccountPublicKey(account.get())), pubKey);
}
