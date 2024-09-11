// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWSolanaAddress.h>

#include <gtest/gtest.h>

TEST(TWSolanaAddress, HDWallet) {
    auto mnemonic =
        "shoot island position soft burden budget tooth cruel issue economy destroy above";
    auto passphrase = "";

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(STRING(mnemonic).get(), STRING(passphrase).get()));

    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), TWCoinTypeSolana, WRAPS(TWCoinTypeDerivationPath(TWCoinTypeSolana)).get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519(privateKey.get()));
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeSolana));
    auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));

    assertStringsEqual(addressStr, "2bUBiBNZyD29gP1oV6de7nxowMLoDBtopMMTGgMvjG5m");
}

TEST(TWSolanaProgram, defaultTokenAddress) {
    const auto solAddress = STRING("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    const auto serumToken = STRING("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");

    auto solanaAddress = WRAP(TWSolanaAddress, TWSolanaAddressCreateWithString(solAddress.get()));
    auto description = WRAPS(TWSolanaAddressDescription(solanaAddress.get()));
    auto tokenAddress = WRAPS(TWSolanaAddressDefaultTokenAddress(solanaAddress.get(), serumToken.get()));

    assertStringsEqual(tokenAddress, "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
    assertStringsEqual(description, "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
}

TEST(TWSolanaProgram, defaultTokenAddressError) {
    const auto solAddress = STRING("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    // Invalid token mint address.
    const auto serumToken = STRING("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKW");

    auto solanaAddress = WRAP(TWSolanaAddress, TWSolanaAddressCreateWithString(solAddress.get()));
    auto description = WRAPS(TWSolanaAddressDescription(solanaAddress.get()));

    EXPECT_EQ(TWSolanaAddressDefaultTokenAddress(solanaAddress.get(), serumToken.get()), nullptr);
}

TEST(TWSolanaProgram, token2022Address) {
    const auto solAddress = STRING("68dzdXkni9BrAwU1asAwurMEdQhXUJq6MNY8niDAny8t");
    const auto catwifhatToken = STRING("7atgF8KQo4wJrD5ATGX7t1V2zVvykPJbFfNeVf1icFv1");

    auto solanaAddress = WRAP(TWSolanaAddress, TWSolanaAddressCreateWithString(solAddress.get()));
    auto description = WRAPS(TWSolanaAddressDescription(solanaAddress.get()));
    auto tokenAddress = WRAPS(TWSolanaAddressToken2022Address(solanaAddress.get(), catwifhatToken.get()));

    assertStringsEqual(tokenAddress, "3PaFQnebQMHBgthRScup2B932cMxA1GBP7m9roCkomHq");
    assertStringsEqual(description, "68dzdXkni9BrAwU1asAwurMEdQhXUJq6MNY8niDAny8t");
}

TEST(TWSolanaProgram, token2022AddressError) {
    const auto solAddress = STRING("68dzdXkni9BrAwU1asAwurMEdQhXUJq6MNY8niDAny8t");
    // Invalid token mint address.
    const auto catwifhatToken = STRING("7atgF8KQo4wJrD5ATGX7t1V2zVvykPJbFfNeVf1icF");

    auto solanaAddress = WRAP(TWSolanaAddress, TWSolanaAddressCreateWithString(solAddress.get()));
    auto description = WRAPS(TWSolanaAddressDescription(solanaAddress.get()));

    EXPECT_EQ(TWSolanaAddressToken2022Address(solanaAddress.get(), catwifhatToken.get()), nullptr);
}
