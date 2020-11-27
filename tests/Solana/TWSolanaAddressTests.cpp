// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../interface/TWTestUtilities.h"

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWSolanaProgram.h>

#include <gtest/gtest.h>

TEST(TWSolanaAddress, HDWallet) {
    auto mnemonic =
        "shoot island position soft burden budget tooth cruel issue economy destroy above";
    auto passphrase = "";

    auto wallet = WRAP(
        TWHDWallet, TWHDWalletCreateWithMnemonic(STRING(mnemonic).get(), STRING(passphrase).get()));

    auto privateKey = TWHDWalletGetKey(wallet.get(), TWCoinTypeSolana, TWCoinTypeDerivationPath(TWCoinTypeSolana));
    auto publicKey = TWPrivateKeyGetPublicKeyEd25519(privateKey);
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey, TWCoinTypeSolana));
    auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));

    assertStringsEqual(addressStr, "2bUBiBNZyD29gP1oV6de7nxowMLoDBtopMMTGgMvjG5m");
}

TEST(TWSolanaProgram, defaultTokenAddress) {
    const char* serumToken = "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt";
    auto address1 = WRAPS(TWSolanaProgramDefaultTokenAddress(TWStringCreateWithUTF8Bytes("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V"), TWStringCreateWithUTF8Bytes(serumToken)));
    EXPECT_EQ(std::string(TWStringUTF8Bytes(address1.get())), "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
}
