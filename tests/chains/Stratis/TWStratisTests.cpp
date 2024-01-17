// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWBitcoinAddress.h>
#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWPrivateKey.h>

#include <gtest/gtest.h>

TEST(Stratis, LegacyAddress) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("a22ddec5c567b4488bb00f69b6146c50da2ee883e2c096db098726394d585730").get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    auto address = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeP2pkhPrefix(TWCoinTypeStratis)));
    auto addressString = WRAPS(TWBitcoinAddressDescription(address.get()));
    assertStringsEqual(addressString, "XMEd53bqmNitpFX1cUd1tV6LRME4pcuaPe");
}

TEST(Stratis, Address) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("55f9cbb0376c422946fa28397c1219933ac60b312ede41bfacaf701ecd546625").get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeStratis));
    auto string = WRAPS(TWAnyAddressDescription(address.get()));

    assertStringsEqual(string, "strax1qytnqzjknvv03jwfgrsmzt0ycmwqgl0as6zdq3n");
}

TEST(Stratis, LockScriptForLegacyAddress) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("XWqnSWzQA5kDAS727UTYtXkdcbKc8mEvyN").get(), TWCoinTypeStratis));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a914d5d068b60f3b63a5a59cc7b8609ac85b76b1896388ac");
}

TEST(Stratis, LockScriptForAddress) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("strax1qqktrryxg23qjxmnhmz9xsp8w4kkfqv7c2xl6t7").get(), TWCoinTypeStratis));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "001405963190c85441236e77d88a6804eeadac9033d8");
}

TEST(Stratis, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    // .bip44
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeStratis, TWHDVersionXPRV));
    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeStratis, TWHDVersionXPUB));

    assertStringsEqual(xprv, "xprv9xyQ71PNhXBFdiY9xAs76X1Y4YzejPv9qe6tKBQ4pEemnmk6b4iFnV1BpJThm2en26emssc558vqHPujDyBKDdSkrNtQiHwbzpQNobWyvh9");
    assertStringsEqual(xpub, "xpub6BxkWWvGXtjYrCcd4CQ7TexGcaq98re1Cs2V7ZogNaBkfa5F8c2WLHKffYrwmJdNQztsd3oJvdmHuhN79c8qKpASRtavBsbcUq1R5SxeQtq");
}

TEST(Stratis, DeriveFromXpub) {
    auto xpub = STRING("xpub6BxkWWvGXtjYrCcd4CQ7TexGcaq98re1Cs2V7ZogNaBkfa5F8c2WLHKffYrwmJdNQztsd3oJvdmHuhN79c8qKpASRtavBsbcUq1R5SxeQtq");
    auto pubKey2 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeStratis, STRING("m/44'/105105'/0'/0/2").get()));
    auto pubKey9 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeStratis, STRING("m/44'/105105'/0'/0/9").get()));

    auto address2 = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithPublicKey(pubKey2.get(), TWCoinTypeP2pkhPrefix(TWCoinTypeStratis)));
    auto address2String = WRAPS(TWBitcoinAddressDescription(address2.get()));

    auto address9 = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithPublicKey(pubKey9.get(), TWCoinTypeP2pkhPrefix(TWCoinTypeStratis)));
    auto address9String = WRAPS(TWBitcoinAddressDescription(address9.get()));

    assertStringsEqual(address2String, "XC4QM1nSbHrLb8sWMf4qXcphocqSAMNLng");
    assertStringsEqual(address9String, "XM4ixdCpyqF86RhKwWRyUXFxXHNypRXiyL");
}