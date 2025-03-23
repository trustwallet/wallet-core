// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include "Pactus/Entry.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <gtest/gtest.h>

namespace TW::Pactus::tests {

TEST(PactusWallet, DerivationPath) {
    auto derivationPath = TWCoinTypeDerivationPath(TWCoinTypePactus);
    assertStringsEqual(WRAPS(derivationPath), "m/44'/21888'/3'/0'");
}

TEST(PactusWallet, HDWallet_MainnetDerivation) {
    auto mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon cactus";
    auto passphrase = "";
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(STRING(mnemonic).get(), STRING(passphrase).get()));

    auto derivationPath1 = TWStringCreateWithUTF8Bytes("m/44'/21888'/3'/0'");
    auto privateKey1 = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), TWCoinTypePactus, derivationPath1));
    auto publicKey1 = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519(privateKey1.get()));
    auto address1 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey1.get(), TWCoinTypePactus));
    auto addressStr1 = WRAPS(TWAnyAddressDescription(address1.get()));

    auto derivationPath2 = TWStringCreateWithUTF8Bytes("m/44'/21888'/3'/1'");
    auto privateKey2 = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), TWCoinTypePactus, derivationPath2));
    auto publicKey2 = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519(privateKey2.get()));
    auto address2 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey2.get(), TWCoinTypePactus));
    auto addressStr2 = WRAPS(TWAnyAddressDescription(address2.get()));

    assertStringsEqual(addressStr1, "pc1rcx9x55nfme5juwdgxd2ksjdcmhvmvkrygmxpa3");
    assertStringsEqual(addressStr2, "pc1r7aynw9urvh66ktr3fte2gskjjnxzruflkgde94");
    TWStringDelete(derivationPath1);
    TWStringDelete(derivationPath2);
}

TEST(PactusWallet, HDWallet_TestnetDerivation) {
    auto mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon cactus";
    auto passphrase = "";
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(STRING(mnemonic).get(), STRING(passphrase).get()));

    auto derivationPath1 = TWStringCreateWithUTF8Bytes("m/44'/21777'/3'/0'");
    auto privateKey1 = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), TWCoinTypePactus, derivationPath1));
    auto publicKey1 = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519(privateKey1.get()));
    auto address1 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKeyDerivation(publicKey1.get(), TWCoinTypePactus, TWDerivationPactusTestnet));
    auto addressStr1 = WRAPS(TWAnyAddressDescription(address1.get()));

    auto derivationPath2 = TWStringCreateWithUTF8Bytes("m/44'/21777'/3'/1'");
    auto privateKey2 = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), TWCoinTypePactus, derivationPath2));
    auto publicKey2 = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519(privateKey2.get()));
    auto address2 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKeyDerivation(publicKey2.get(), TWCoinTypePactus, TWDerivationPactusTestnet));
    auto addressStr2 = WRAPS(TWAnyAddressDescription(address2.get()));

    assertStringsEqual(addressStr1, "tpc1r35xwz99uw2qrhz9wmdanaqcsge2nzsfegvv555");
    assertStringsEqual(addressStr2, "tpc1r34xj32k004j8v35fx6uqw4yaka54g6jdr58tvk");
    TWStringDelete(derivationPath1);
    TWStringDelete(derivationPath2);
}

}