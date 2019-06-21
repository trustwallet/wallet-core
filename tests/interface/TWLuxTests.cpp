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
#include <TrustWalletCore/TWPrivateKey.h>

#include <gtest/gtest.h>

TEST(Lux, Address) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("a22ddec5c567b4488bb00f69b6146c50da2ee883e2c096db098726394d585730").get()));
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true);
    auto address = TWBitcoinAddressCreateWithPublicKey(publicKey, TWCoinTypeP2pkhPrefix(TWCoinTypeLux));
    auto addressString = WRAPS(TWBitcoinAddressDescription(address));
    assertStringsEqual(addressString, "LV7LV7Z4bWDEjYkfx9dQo6k6RjGbXsg6hS");
}

TEST(Lux, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeLux, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeLux, TWHDVersionXPRV));

    assertStringsEqual(xpub, "xpub6CPmhSbYFoVy6rr2eGGkK7nbJYTL8SoWZYs3uSuWRpUg1s9Ln3xmEjMyycjvsF8qF76UEqh4yvmdCBbtXPKatMdrvo5bdhchvQqEZw4anDC");
    assertStringsEqual(xprv, "xprv9yQRHw4eRRwftNmZYEjjwyqrkWcqiz5fCKwT74VtsUwh94pCEWeWgw3W8LEC1BVF5r6bJLuCnD6HR2X8bETk3mw5VBgVDuw2BNL2NAVGp5y");
}

TEST(Lux, DeriveFromXpub) {
    auto xpub = STRING("xpub6Cb8Q6pDeS8PdKNbDv9Hvq4WpJXL3JvKvmHHwR1wD2H543hiCUE1f1tB5AXE6yg13k7xZ6PzEXMNUFHXk6kkx4RYte8VB1i4tCX9rwQVR4a");
    auto pubKey3 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/3003'/0'/0/3").get());
    auto pubKey5 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/3003'/0'/0/5").get());

    auto address3 = TWBitcoinAddressCreateWithPublicKey(pubKey3, TWCoinTypeP2pkhPrefix(TWCoinTypeLux));
    auto address3String = WRAPS(TWBitcoinAddressDescription(address3));

    auto address5 = TWBitcoinAddressCreateWithPublicKey(pubKey5, TWCoinTypeP2pkhPrefix(TWCoinTypeLux));
    auto address5String = WRAPS(TWBitcoinAddressDescription(address5));

    assertStringsEqual(address3String, "LfJVQc6oq2T19WQLHAvMeHUjZVsspKFQu4");
    assertStringsEqual(address5String, "LdEXFwEC9qZTVbVJdhNzHmrQBmUmG4VxzL");
}

TEST(Lux, LockScripts) {
    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("LcWJv3djruGY4uh7xVPZyKxqJJUTdrzqL7").get(), TWCoinTypeLux));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "76a914bd92088bb7e82d611a9b94fbb74a0908152b784f88ac");

    auto script3 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("SfZ3YWaVKwMgfvgUGg6AE3jXY5HEf4g1XU").get(), TWCoinTypeLux));
    auto scriptData3 = WRAPD(TWBitcoinScriptData(script3.get()));
    assertHexEqual(scriptData3, "a914c84a999d5df4a6a8e06908c5e35a9cb1260d834587");
}
