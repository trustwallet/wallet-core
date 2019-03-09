// Copyright © 2017-2019 Trust Wallet.
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

TEST(Zcash, TransparentAddress) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("987919d988ef94e678bce254c932e7a7a76744b2c008467448406d4246513132").get()));
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true);
    auto address = TWZcashTAddress();
    TWZcashTAddressInitWithPublicKey(&address, publicKey, TWP2PKHPrefixZcashT);
    auto addressString = WRAPS(TWZcashTAddressDescription(address));
    assertStringsEqual(addressString, "t1RygJmrLdNGgi98gUgEJDTVaELTAYWoMBy");
}

TEST(Zcash, DeriveTransparentAddress) {
    auto words = STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal");
    auto passphrase = STRING("TREZOR");

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), TWCoinTypeZcash, 0, 0, 5));
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(key.get(), false);

    TWZcashTAddress address;
    TWZcashTAddressInitWithPublicKey(&address, publicKey, TWP2PKHPrefixZcashT);
    auto addressString = WRAPS(TWZcashTAddressDescription(address));

    assertStringsEqual(addressString, "t1TWk2mmvESDnE4dmCfT7MQ97ij6ZqLpNVU");
}

TEST(Zcash, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeZcash, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeZcash, TWHDVersionXPRV));

    assertStringsEqual(xpub, "xpub6CksSgKBhD9KaLgxLE9LXpSj74b2EB9d1yKvhWxrstk4Md8gmiJb5GwkMeBhpLxVjACMdNbRsAm2GG5ehVuyq42QBYYPAjXjcBxMVmpaaNL");
    assertStringsEqual(xprv, "xprv9ymX3AnHrqb2MrcVECcLAgVzZ2kXpiRmekQKu8ZFKZD5UpoYEAzLXUdGWPZJn1mwWn8pEmqKh97KwdE2r3PCQAnoWAPEThMCLEKSPK3NgRS");
}

TEST(Zcash, DerivePubkeyFromXpub) {
    auto xpub = STRING("xpub6CksSgKBhD9KaLgxLE9LXpSj74b2EB9d1yKvhWxrstk4Md8gmiJb5GwkMeBhpLxVjACMdNbRsAm2GG5ehVuyq42QBYYPAjXjcBxMVmpaaNL");
    auto pubKey3 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCurveSECP256k1, TWHDVersionXPUB, TWHDVersionXPRV, 0, 3);
    auto pubKey5 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCurveSECP256k1, TWHDVersionXPUB, TWHDVersionXPRV, 0, 5);

    TWZcashTAddress address3;
    TWZcashTAddressInitWithPublicKey(&address3, pubKey3, TWP2PKHPrefixZcashT);
    auto address3String = WRAPS(TWZcashTAddressDescription(address3));

    TWZcashTAddress address5;
    TWZcashTAddressInitWithPublicKey(&address5, pubKey5, TWP2PKHPrefixZcashT);
    auto address5String = WRAPS(TWZcashTAddressDescription(address5));

    assertStringsEqual(address3String, "t1cWhcXydPYTG1pgHMsZ6JEPsWGxVMdJ5t6");
    assertStringsEqual(address5String, "t1TWk2mmvESDnE4dmCfT7MQ97ij6ZqLpNVU");
}

TEST(Zcash, DeriveAddressFromXpub) {
    auto xpub = STRING("xpub6C7HhMqpir3KBA6ammv5B58RT3XFTJqoZFoj3J56dz9XwehZ2puSH38ERtnz7HaXGxaZP8AHT4M2bSRHpBXUZrbsJ2xg3xs53DGKYCqj8mr");
    auto address = WRAPS(TWHDWalletGetAddressFromExtended(xpub.get(), TWCurveSECP256k1, TWCoinTypeZcash, 0, 0));

    assertStringsEqual(address, "t1TKCtCETHPrAdA6eY1fdhhnTkTmb371oPt");
}
