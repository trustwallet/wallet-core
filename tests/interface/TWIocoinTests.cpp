// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWIocoinAddress.h>
#include <TrustWalletCore/TWBitcoinScript.h>
#include <gtest/gtest.h>

TEST(Iocoin, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeIocoin, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeIocoin, TWHDVersionXPRV));

    assertStringsEqual(xpub, "xpub6DHj382cGmV2xBnzy7KaLLBqac3qwaBcHZL1BLzjaNH9R6kye9ix45TBHjBYvS9MdfQFWtSSctJZpdF6wm4pnN1eikCvJ7N9NRAZwGyFipn");
    assertStringsEqual(xprv, "xprv9zJNdcViSPvjjhiXs5nZyCF72aDMY7TkvLQQNxb822kAYJRq6cQhWH8hSV9Tcq9V4WyvoMVhu5tm9BcUjb3LqTaLWvwC6QfnTkUDwbERbph");
}

TEST(Iocoin, DeriveFromXpub) {
    auto xpub = STRING("xpub6DHj382cGmV2xBnzy7KaLLBqac3qwaBcHZL1BLzjaNH9R6kye9ix45TBHjBYvS9MdfQFWtSSctJZpdF6wm4pnN1eikCvJ7N9NRAZwGyFipn");
    auto pubKey0 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/295'/0'/0/0").get());
    auto address0 = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeIocoin, pubKey0));

    assertStringsEqual(address0, "idi85QBDyPiLyi89HvAA4jY1hJS66M3wsL");
}

TEST(Iocoin, LockScripts) {
    auto script1 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("igtYaw43AsogbQTMGDJPXrpV1phPREmKkJ").get()));
    auto scriptData1 = WRAPD(TWBitcoinScriptData(script1.get()));
    assertHexEqual(scriptData1, "76a9149a67996bb753f53b8e2f13cd15fda2722f2f245888ac");

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("iUfZgENcr6KRY8W88CTF1oBMBbrstm8xTt").get()));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "76a914145197e9a8549072610b3515427553848c6d0d0388ac");
    auto script3 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("bMWq7CWqBETShngqiRQpbUC2mcqT8AKxsx").get()));
    auto scriptData3 = WRAPD(TWBitcoinScriptData(script3.get()));
    assertHexEqual(scriptData3, "a914605ec8221d996ebfb6369b56c16e7d91369be43887");
}
