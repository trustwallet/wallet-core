// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWSegwitAddress.h>
#include <TrustWalletCore/TWBitcoinAddress.h>
#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWP2PKHPrefix.h>
#include <TrustWalletCore/TWP2SHPrefix.h>
#include <TrustWalletCore/TWPrivateKey.h>

#include <gtest/gtest.h>

TEST(Nebulas, Address) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("d2fd0ec9f6268fc8d1f563e3e976436936708bdf0dc60c66f35890f5967a8d2b").get()));
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), false);
    auto address = TWBitcoinAddressCreateWithPublicKey(publicKey, TWP2PKHPrefixNebulas);
    auto addressString = WRAPS(TWBitcoinAddressDescription(address));
    assertStringsEqual(addressString, "aAbqxogrjdy2YHVcnQxFHMzqpt2fhjCTVT");
}

TEST(Nebulas, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeNebulas, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeNebulas, TWHDVersionXPRV));

    assertStringsEqual(xpub, "xpub6Cb8Q6pDeS8PdKNbDv9Hvq4WpJXL3JvKvmHHwR1wD2H543hiCUE1f1tB5AXE6yg13k7xZ6PzEXMNUFHXk6kkx4RYte8VB1i4tCX9rwQVR4a");
    assertStringsEqual(xprv, "xprv9ybmzbHKp4a6QqJ87tcHZh7nGGgqdrCUZYMh92cKegk6BFNZevum7DZhDuVDqqMdcBT9B4wJSEmwJW9JNdkMcUUjEWKqppxNrJjKFSyKsCr");
}

TEST(Nebulas, DeriveFromXpub) {
    auto xpub = STRING("xpub6Cb8Q6pDeS8PdKNbDv9Hvq4WpJXL3JvKvmHHwR1wD2H543hiCUE1f1tB5AXE6yg13k7xZ6PzEXMNUFHXk6kkx4RYte8VB1i4tCX9rwQVR4a");
    auto pubKey3 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/2718'/0'/0/3").get());
    auto pubKey5 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/2718'/0'/0/5").get());

    auto address3 = TWBitcoinAddressCreateWithPublicKey(pubKey3, TWP2PKHPrefixNebulas);
    auto address3String = WRAPS(TWBitcoinAddressDescription(address3));

    auto address5 = TWBitcoinAddressCreateWithPublicKey(pubKey5, TWP2PKHPrefixNebulas);
    auto address5String = WRAPS(TWBitcoinAddressDescription(address5));

    assertStringsEqual(address3String, "aLnztJEbyACnxF9H7SFC8YjUxedwyQsgVm");
    assertStringsEqual(address5String, "aJj2jdMzHyKFJLEFTxhpn379avEqRKFUyw");
}

TEST(Nebulas, LockScripts) {
    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("a4YtT82mWWxHZhLmdx7e5aroW92dqJoRs3").get()));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "76a9142a10f88e30768d2712665c279922b9621ce58bc788ac");

    auto script3 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("4CFa4fnAQvFz4VpikGNzQ9XfCDXMmdk6sh").get()));
    auto scriptData3 = WRAPD(TWBitcoinScriptData(script3.get()));
    assertHexEqual(scriptData3, "a914f010b17a9189e0f2737d71ae9790359eb5bbc13787");
}
