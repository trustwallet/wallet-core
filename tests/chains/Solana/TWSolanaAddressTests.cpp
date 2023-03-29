// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
