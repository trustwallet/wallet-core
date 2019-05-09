// Copyright Â© 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWP2PKHPrefix.h>
#include <TrustWalletCore/TWP2SHPrefix.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWPurpose.h>
#include <TrustWalletCore/TWZcashTAddress.h>

#include <gtest/gtest.h>

TEST(Zelcash, TransparentAddress) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("987919d988ef94e678bce254c932e7a7a76744b2c008467448406d4246513132").get()));
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true);
    auto address = TWZcashTAddressCreateWithPublicKey(publicKey, TWP2PKHPrefixZcashT);
    auto addressString = WRAPS(TWZcashTAddressDescription(address));
    assertStringsEqual(addressString, "t1RygJmrLdNGgi98gUgEJDTVaELTAYWoMBy");
}

TEST(Zelcash, DeriveTransparentAddress) {
    auto words = STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal");
    auto passphrase = STRING("TREZOR");

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto derivationPath = STRING("m/44'/19167'/0'/0/5");
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), derivationPath.get()));
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(key.get(), true);

    auto address = TWZcashTAddressCreateWithPublicKey(publicKey, TWP2PKHPrefixZcashT);
    auto addressString = WRAPS(TWZcashTAddressDescription(address));

    assertStringsEqual(addressString, "t1Trs2rNPzL4Jm24foTd89KpPWqLtLSciDY");
}

TEST(Zelcash, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeZelcash, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeZelcash, TWHDVersionXPRV));

    assertStringsEqual(xpub, "xpub6DATuScKPEk6YvULrHPff1NKC49nyz5mCZQyxSDEQihq3kfoKDYCznLrsdW4KmXw9TryNfEZ9JSD8tJL9UTC3LnBA54YZL7nqMtJm7Ffnoz");
    assertStringsEqual(xprv, "xprv9zB7Vw5RYsBoLSPskFrfHsRae2KJaXMuqLVPA3ocrPArAxLemgDxSz2P2NdreeXKs8N3BxbVPHXeiaamtWC4zoa2Ff8toqa7NEbNWH32Ctb");
}

TEST(Zelcash, DerivePubkeyFromXpub) {
    auto xpub = STRING("xpub6DATuScKPEk6YvULrHPff1NKC49nyz5mCZQyxSDEQihq3kfoKDYCznLrsdW4KmXw9TryNfEZ9JSD8tJL9UTC3LnBA54YZL7nqMtJm7Ffnoz");
    auto pubKey3 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/19167'/0'/0/3").get());
    auto pubKey5 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/19167'/0'/0/5").get());

    auto address3 = TWZcashTAddressCreateWithPublicKey(pubKey3, TWP2PKHPrefixZcashT);
    auto address3String = WRAPS(TWZcashTAddressDescription(address3));

    auto address5 = TWZcashTAddressCreateWithPublicKey(pubKey5, TWP2PKHPrefixZcashT);
    auto address5String = WRAPS(TWZcashTAddressDescription(address5));

    assertStringsEqual(address3String, "t1NdSKKkBXV3GBDMcPvpWu12qcNwAZwB4hD");
    assertStringsEqual(address5String, "t1Trs2rNPzL4Jm24foTd89KpPWqLtLSciDY");
}

TEST(Zelcash, DerivePubkeyFromXpub2) {
    auto xpub = STRING("xpub6C7HhMqpir3KBA6ammv5B58RT3XFTJqoZFoj3J56dz9XwehZ2puSH38ERtnz7HaXGxaZP8AHT4M2bSRHpBXUZrbsJ2xg3xs53DGKYCqj8mr");
    auto pubKey = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/19167'/0'/0/0").get());
    auto address = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeZcash, pubKey));
    assertStringsEqual(address, "t1TKCtCETHPrAdA6eY1fdhhnTkTmb371oPt");
}
