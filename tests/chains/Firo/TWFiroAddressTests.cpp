// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWDerivation.h>
#include <TrustWalletCore/TWFiroAddressType.h>
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

TEST(TWZCoin, ExchangeAddress_CreateWithString) {
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(STRING("aJtPAs49k2RYonsUoY9SGgmpzv4awdPfVP").get(), TWCoinTypeFiro));
    auto addressData = WRAPD(TWAnyAddressData(address.get()));
    assertHexEqual(addressData, "c7529bf17541410428c7b23b402761acb83fdfba");

    auto exchangeAddress = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(STRING("EXXYdhSMM9Em5Z3kzdUWeUm2vFMNyXFSAEE9").get(), TWCoinTypeFiro));
    auto exchangeAddressData = WRAPD(TWAnyAddressData(exchangeAddress.get()));
    assertHexEqual(exchangeAddressData, "c7529bf17541410428c7b23b402761acb83fdfba");
}

TEST(TWZCoin, ExchangeAddress_DeriveFromPublicKey) {
    auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA("034cc1963365aa67d35643f419d6601eca6ef7f62e46bf7f8b6ffa64e2f44fd0bf").get(), TWPublicKeyTypeSECP256k1));
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKeyFiroAddressType(publicKey.get(), TWFiroAddressTypeExchange));
    auto addressDesc = WRAPS(TWAnyAddressDescription(address.get()));
    assertStringsEqual(addressDesc, "EXXWKhUtcaFKVW1NeRFuqPq33zAJMtQJwR4y");

    auto defaultAddress = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKeyFiroAddressType(publicKey.get(), TWFiroAddressTypeDefault));
    auto defaultAddressDesc = WRAPS(TWAnyAddressDescription(defaultAddress.get()));
    assertStringsEqual(defaultAddressDesc, "aGaPDQKakaqVmQXGawLMLguZoqSx6CnSfK");
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
