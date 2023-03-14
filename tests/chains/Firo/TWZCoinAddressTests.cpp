// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include <TrustWalletCore/TWSegwitAddress.h>
#include <TrustWalletCore/TWBitcoinAddress.h>
#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWPrivateKey.h>

#include <gtest/gtest.h>

TEST(TWZCoin, Address) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("a22ddec5c567b4488bb00f69b6146c50da2ee883e2c096db098726394d585730").get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    auto address = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeP2pkhPrefix(TWCoinTypeFiro)));
    auto addressString = WRAPS(TWBitcoinAddressDescription(address.get()));
    assertStringsEqual(addressString, "aAbqxogrjdy2YHVcnQxFHMzqpt2fhjCTVT");
}

TEST(TWZCoin, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeFiro, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeFiro, TWHDVersionXPRV));

    assertStringsEqual(xpub, "xpub6Cb8Q6pDeS8PdKNbDv9Hvq4WpJXL3JvKvmHHwR1wD2H543hiCUE1f1tB5AXE6yg13k7xZ6PzEXMNUFHXk6kkx4RYte8VB1i4tCX9rwQVR4a");
    assertStringsEqual(xprv, "xprv9ybmzbHKp4a6QqJ87tcHZh7nGGgqdrCUZYMh92cKegk6BFNZevum7DZhDuVDqqMdcBT9B4wJSEmwJW9JNdkMcUUjEWKqppxNrJjKFSyKsCr");
}

TEST(TWZcoin, DeriveFromXpub) {
    auto xpub = STRING("xpub6Cb8Q6pDeS8PdKNbDv9Hvq4WpJXL3JvKvmHHwR1wD2H543hiCUE1f1tB5AXE6yg13k7xZ6PzEXMNUFHXk6kkx4RYte8VB1i4tCX9rwQVR4a");
    auto pubKey3 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeFiro, STRING("m/44'/136'/0'/0/3").get()));
    auto pubKey5 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeFiro, STRING("m/44'/136'/0'/0/5").get()));

    auto address3 = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithPublicKey(pubKey3.get(), TWCoinTypeP2pkhPrefix(TWCoinTypeFiro)));
    auto address3String = WRAPS(TWBitcoinAddressDescription(address3.get()));

    auto address5 = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithPublicKey(pubKey5.get(), TWCoinTypeP2pkhPrefix(TWCoinTypeFiro)));
    auto address5String = WRAPS(TWBitcoinAddressDescription(address5.get()));

    assertStringsEqual(address3String, "aLnztJEbyACnxF9H7SFC8YjUxedwyQsgVm");
    assertStringsEqual(address5String, "aJj2jdMzHyKFJLEFTxhpn379avEqRKFUyw");
}

TEST(TWZcoin, LockScripts) {
    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("a4YtT82mWWxHZhLmdx7e5aroW92dqJoRs3").get(), TWCoinTypeFiro));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "76a9142a10f88e30768d2712665c279922b9621ce58bc788ac");

    auto script3 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("4CFa4fnAQvFz4VpikGNzQ9XfCDXMmdk6sh").get(), TWCoinTypeFiro));
    auto scriptData3 = WRAPD(TWBitcoinScriptData(script3.get()));
    assertHexEqual(scriptData3, "a914f010b17a9189e0f2737d71ae9790359eb5bbc13787");
}
