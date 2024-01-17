// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWPurpose.h>
#include <TrustWalletCore/TWAnyAddress.h>

#include <gtest/gtest.h>

TEST(TWZelcash, TransparentAddress) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("987919d988ef94e678bce254c932e7a7a76744b2c008467448406d4246513132").get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeZelcash));
    auto addressString = WRAPS(TWAnyAddressDescription(address.get()));
    assertStringsEqual(addressString, "t1RygJmrLdNGgi98gUgEJDTVaELTAYWoMBy");
}

TEST(TWZelcash, DeriveTransparentAddress) {
    auto words = STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal");
    auto passphrase = STRING("TREZOR");

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto derivationPath = STRING("m/44'/19167'/0'/0/5");
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), TWCoinTypeZelcash, derivationPath.get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key.get(), true));

    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeZelcash));
    auto addressString = WRAPS(TWAnyAddressDescription(address.get()));

    assertStringsEqual(addressString, "t1Trs2rNPzL4Jm24foTd89KpPWqLtLSciDY");
}

TEST(TWZelcash, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeZelcash, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeZelcash, TWHDVersionXPRV));

    assertStringsEqual(xpub, "xpub6DATuScKPEk6YvULrHPff1NKC49nyz5mCZQyxSDEQihq3kfoKDYCznLrsdW4KmXw9TryNfEZ9JSD8tJL9UTC3LnBA54YZL7nqMtJm7Ffnoz");
    assertStringsEqual(xprv, "xprv9zB7Vw5RYsBoLSPskFrfHsRae2KJaXMuqLVPA3ocrPArAxLemgDxSz2P2NdreeXKs8N3BxbVPHXeiaamtWC4zoa2Ff8toqa7NEbNWH32Ctb");
}

TEST(TWZelcash, DerivePubkeyFromXpub) {
    auto xpub = STRING("xpub6DATuScKPEk6YvULrHPff1NKC49nyz5mCZQyxSDEQihq3kfoKDYCznLrsdW4KmXw9TryNfEZ9JSD8tJL9UTC3LnBA54YZL7nqMtJm7Ffnoz");
    auto pubKey3 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeZelcash, STRING("m/44'/19167'/0'/0/3").get()));
    auto pubKey5 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeZelcash, STRING("m/44'/19167'/0'/0/5").get()));

    auto address3 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubKey3.get(), TWCoinTypeZelcash));
    auto address3String = WRAPS(TWAnyAddressDescription(address3.get()));

    auto address5 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubKey5.get(), TWCoinTypeZelcash));
    auto address5String = WRAPS(TWAnyAddressDescription(address5.get()));

    assertStringsEqual(address3String, "t1NdSKKkBXV3GBDMcPvpWu12qcNwAZwB4hD");
    assertStringsEqual(address5String, "t1Trs2rNPzL4Jm24foTd89KpPWqLtLSciDY");
}

TEST(TWZelcash, DerivePubkeyFromXpub2) {
    auto xpub = STRING("xpub6C7HhMqpir3KBA6ammv5B58RT3XFTJqoZFoj3J56dz9XwehZ2puSH38ERtnz7HaXGxaZP8AHT4M2bSRHpBXUZrbsJ2xg3xs53DGKYCqj8mr");
    auto pubKey = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeZelcash, STRING("m/44'/19167'/0'/0/0").get()));
    auto address = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeZelcash, pubKey.get()));
    assertStringsEqual(address, "t1TKCtCETHPrAdA6eY1fdhhnTkTmb371oPt");
}
