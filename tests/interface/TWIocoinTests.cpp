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

    auto dpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeIocoin, TWHDVersionDPUB));
    auto dprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeIocoin, TWHDVersionDPRV));

    assertStringsEqual(dpub, "dpubZGHTD5JbfpgQVUy4Gnu4XkEoPC185GDMik8YFNXeozB5Ewz8B4ccmfA6CFdw2hzYaNBTimiFPJZ6wVNrN7DjwQn2N86sGVYcu5MGoe2E83L");
    assertStringsEqual(dprv, "dprv3pVMqwkJbSccb55J2Gqi1XSYmBSreKdHXCWDWTbHouaSXPxVJ7H1h5e53ZwKeaizKMkcZKB5AXAKLjxm5MB5gybv2Uh8z77XJyGyYbMsQy8");
}

TEST(Iocoin, DerivePubkeyFromDpub) {
    auto dpub = STRING("dpubZGHTD5JbfpgQVUy4Gnu4XkEoPC185GDMik8YFNXeozB5Ewz8B4ccmfA6CFdw2hzYaNBTimiFPJZ6wVNrN7DjwQn2N86sGVYcu5MGoe2E83L");
    auto pubKey0 = TWHDWalletGetPublicKeyFromExtended(dpub.get(), TWCoinTypeIocoin, TWHDVersionDPUB, TWHDVersionDPRV, 0, 0);
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
}
