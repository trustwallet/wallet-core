// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include "Zcash/TAddress.h"

#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWPurpose.h>
#include <TrustWalletCore/TWAnyAddress.h>

#include <gtest/gtest.h>

TEST(TWZcash, TransparentAddress) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("987919d988ef94e678bce254c932e7a7a76744b2c008467448406d4246513132").get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeZcash));
    auto addressString = WRAPS(TWAnyAddressDescription(address.get()));
    assertStringsEqual(addressString, "t1RygJmrLdNGgi98gUgEJDTVaELTAYWoMBy");
}

TEST(TWZcash, DeriveTransparentAddress) {
    auto words = STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal");
    auto passphrase = STRING("TREZOR");

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto derivationPath = STRING("m/44'/133'/0'/0/5");
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), TWCoinTypeZcash, derivationPath.get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key.get(), true));

    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeZcash));
    auto addressString = WRAPS(TWAnyAddressDescription(address.get()));

    assertStringsEqual(addressString, "t1TWk2mmvESDnE4dmCfT7MQ97ij6ZqLpNVU");
}

TEST(TWZcash, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeZcash, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeZcash, TWHDVersionXPRV));

    assertStringsEqual(xpub, "xpub6CksSgKBhD9KaLgxLE9LXpSj74b2EB9d1yKvhWxrstk4Md8gmiJb5GwkMeBhpLxVjACMdNbRsAm2GG5ehVuyq42QBYYPAjXjcBxMVmpaaNL");
    assertStringsEqual(xprv, "xprv9ymX3AnHrqb2MrcVECcLAgVzZ2kXpiRmekQKu8ZFKZD5UpoYEAzLXUdGWPZJn1mwWn8pEmqKh97KwdE2r3PCQAnoWAPEThMCLEKSPK3NgRS");
}

TEST(TWZcash, DerivePubkeyFromXpub) {
    auto xpub = STRING("xpub6CksSgKBhD9KaLgxLE9LXpSj74b2EB9d1yKvhWxrstk4Md8gmiJb5GwkMeBhpLxVjACMdNbRsAm2GG5ehVuyq42QBYYPAjXjcBxMVmpaaNL");
    auto pubKey3 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeZcash, STRING("m/44'/133'/0'/0/3").get()));
    auto pubKey5 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeZcash, STRING("m/44'/133'/0'/0/5").get()));

    auto address3 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubKey3.get(), TWCoinTypeZcash));
    auto address3String = WRAPS(TWAnyAddressDescription(address3.get()));

    auto address5 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubKey5.get(), TWCoinTypeZcash));
    auto address5String = WRAPS(TWAnyAddressDescription(address5.get()));

    assertStringsEqual(address3String, "t1cWhcXydPYTG1pgHMsZ6JEPsWGxVMdJ5t6");
    assertStringsEqual(address5String, "t1TWk2mmvESDnE4dmCfT7MQ97ij6ZqLpNVU");
}

TEST(TWZcash, DerivePubkeyFromXpub2) {
    auto xpub = STRING("xpub6C7HhMqpir3KBA6ammv5B58RT3XFTJqoZFoj3J56dz9XwehZ2puSH38ERtnz7HaXGxaZP8AHT4M2bSRHpBXUZrbsJ2xg3xs53DGKYCqj8mr");
    auto pubKey = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeZcash, STRING("m/44'/133'/0'/0/0").get()));
    auto address = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeZcash, pubKey.get()));
    assertStringsEqual(address, "t1TKCtCETHPrAdA6eY1fdhhnTkTmb371oPt");
}

TEST(TWZcash, LockScripts) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("t1bjVPEY8NbpGxT2PgayX3HevfJ2YU5X2DS").get(), TWCoinTypeZcash));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a914c3e968851fdb2bb943662befdb8b8573ecd4d08e88ac");

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("t3gQDEavk5VzAAHK8TrQu2BWDLxEiF1unBm").get(), TWCoinTypeZcash));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "a914ef8b3e86db855eb48bcf0b7585a90b6b9ece75c087");
}
