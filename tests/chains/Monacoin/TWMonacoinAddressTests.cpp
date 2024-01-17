// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/TWSegwitAddress.h>
#include <TrustWalletCore/TWBitcoinAddress.h>
#include <TrustWalletCore/TWBitcoinScript.h>

#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWPrivateKey.h>

#include <gtest/gtest.h>

TEST(Monacoin, LegacyAddress) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("a22ddec5c567b4488bb00f69b6146c50da2ee883e2c096db098726394d585730").get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    auto address = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeP2pkhPrefix(TWCoinTypeMonacoin)));
    auto addressString = WRAPS(TWBitcoinAddressDescription(address.get()));
    assertStringsEqual(addressString, "MHnYTL9e1s8zNR2qzzJ3mMHfgjnUzyMscd");
}

TEST(Monacoin, Address) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("55f9cbb0376c422946fa28397c1219933ac60b312ede41bfacaf701ecd546625").get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    auto address = WRAP(TWSegwitAddress, TWSegwitAddressCreateWithPublicKey(TWHRPMonacoin, publicKey.get()));
    auto string = WRAPS(TWSegwitAddressDescription(address.get()));

    assertStringsEqual(string, "mona1qytnqzjknvv03jwfgrsmzt0ycmwqgl0asju3qmd");
}

TEST(Monacoin, BuildForP2PKHAddress) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("MFMy9FwJsV6HiN5eZDqDETw4pw52q3UGrb").get(), TWCoinTypeMonacoin));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a91451dadacc7021440cbe4ca148a5db563b329b4c0388ac");
}

TEST(Monacoin, BuildForP2SHAddress) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("PHjTKtgYLTJ9D2Bzw2f6xBB41KBm2HeGfg").get(), TWCoinTypeMonacoin));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "a9146449f568c9cd2378138f2636e1567112a184a9e887");
}

TEST(Monacoin, BuildForBech32Address) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("mona1q4kpn6psthgd5ur894auhjj2g02wlgmp8ke08ne").get(), TWCoinTypeMonacoin));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "0014ad833d060bba1b4e0ce5af797949487a9df46c27");
}

TEST(Monacoin, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    // .bip44
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeMonacoin, TWHDVersionXPRV));
    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeMonacoin, TWHDVersionXPUB));

    assertStringsEqual(xprv, "xprv9ySV9MzJnFMm7bi5qVTcqva3oDJojNsMYuASBcj9yC4QRe4kehWYeGfUVYFiJQipELCHmiutHJVxosR7eSovWHrWCUTdhf49D1N7MGKVcUZ");
    assertStringsEqual(xpub, "xpub6CRqYsXCccv4L5nYwWzdD4WnMF9J8qbCv862z18mXXbPJSPuCEpoC4yxLn5N81V5GWNuBsJ8g6tYsBb1V9hCipGn4WR6bc9FLUtyyRvDFse");

    // .bip49
    auto yprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP49, TWCoinTypeMonacoin, TWHDVersionYPRV));
    auto ypub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP49, TWCoinTypeMonacoin, TWHDVersionYPUB));

    assertStringsEqual(yprv, "yprvAJL1swHSWeFvfKdupcrsAvTCrtAiKUhLrUjteTj8JGCzD53YrJgiVbsChMPi5h119cn5tVVk1QAFSJtcnMSSxjGCNDXg8YaWbe4Hhc5bUfL");
    assertStringsEqual(ypub, "ypub6XKNHSpLM1pDsoiNvePsY4PwQv1CiwRCDhfVSr8jrbjy5sNhPqzy3QBgYdCayJhq5st63XZTWrea8So84QYbPgP2EvVR5dhSrW18ud4GZaT");

    // .bip84
    auto zprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP84, TWCoinTypeMonacoin, TWHDVersionZPRV));
    auto zpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP84, TWCoinTypeMonacoin, TWHDVersionZPUB));
    assertStringsEqual(zprv, "zprvAdi4KQxm5ofVJqh1Y5KxSTuC1CSEibkz3Ei4dnNMzyLt6FiW3rMPTgKUCiwv1cw4rVeDW9ju82ChFz27UNG2kxvtFsngJCYtHjDYURsefzX");
    assertStringsEqual(zpub, "zpub6rhQivVevBDnXKmUe6rxobqvZEGj84UqQTdfSAmyZJsry43ebPfe1Udx3zUNDB3cHu2ozNCDhsy8BuNCjCvStNmodzdR2E2wWAsLyNFu5i1");
}

TEST(Monacoin, DeriveFromXpub) {
    auto xpub = STRING("xpub6CYWFE1BgTCW2vtbDm1RRT81i3hBkQrXCfGs5hYp211fpgLZV5xCEwXMWPAL3LgaBA9koXpLZSUo7rTyJ8q1JwqKhvzVpdzBKRGyyGb31KF");
    auto pubKey2 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeMonacoin, STRING("m/44'/22'/0'/0/2").get()));
    auto pubKey9 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeMonacoin, STRING("m/44'/22'/0'/0/9").get()));

    auto address2 = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithPublicKey(pubKey2.get(), TWCoinTypeP2pkhPrefix(TWCoinTypeMonacoin)));
    auto address2String = WRAPS(TWBitcoinAddressDescription(address2.get()));

    auto address9 = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithPublicKey(pubKey9.get(), TWCoinTypeP2pkhPrefix(TWCoinTypeMonacoin)));
    auto address9String = WRAPS(TWBitcoinAddressDescription(address9.get()));

    assertStringsEqual(address2String, "MCoYzbqdsMYTBbjr7rd2zJsSF32QMgZCSj");
    assertStringsEqual(address9String, "MAtduu1Fvtv1Frx6vbg5tZDZwirCA3y8qq");
}

TEST(Monacoin, LockScripts) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("mona1qw508d6qejxtdg4y5r3zarvary0c5xw7kg5lnx5").get(), TWCoinTypeMonacoin));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "0014751e76e8199196d454941c45d1b3a323f1433bd6");

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("PCTzdjWauNipkYtToRZEHDMXb2adj9Evp8").get(), TWCoinTypeMonacoin));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "a9142a84cf00d47f699ee7bbc1dea5ec1bdecb4ac15487");

    auto script3 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("MBamfEqEFDy5dsLWwu48BCizM1zpCoKw3U").get(), TWCoinTypeMonacoin));
    auto scriptData3 = WRAPD(TWBitcoinScriptData(script3.get()));
    assertHexEqual(scriptData3, "76a91428662c67561b95c79d2257d2a93d9d151c977e9188ac");
}
