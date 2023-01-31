// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"
#include "Nebl/TAddress.h"

#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWPurpose.h>
#include <TrustWalletCore/TWAnyAddress.h>
#include <gtest/gtest.h>

TEST(TWNeblAddress, TransparentAddress) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("4222aae79af41eade7b07ce6fd44d926ea8e3f95e51a06e85f8bdec89680cbd9").get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeNebl));
    auto addressString = WRAPS(TWAnyAddressDescription(address.get()));
    assertStringsEqual(addressString, "NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc");
}

TEST(TWNeblAddress, DeriveTransparentAddress) {
    auto words = STRING("evil title scatter trouble uphold staff elder canoe gossip ski eyebrow anger loop myself toast");
    auto passphrase = STRING("NEBL");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto derivationPath = STRING("m/44'/0'/0'/0/0");
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), TWCoinTypeNebl, derivationPath.get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key.get(), true));

    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeNebl));
    auto addressString = WRAPS(TWAnyAddressDescription(address.get()));

    assertStringsEqual(addressString, "NZV9g3TX7FLqnCFgXw5B2Rj2NQj7n8vhju");
}

TEST(TWNeblAddress, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("evil title scatter trouble uphold staff elder canoe gossip ski eyebrow anger loop myself toast").get(),
        STRING("NEBL").get()
    ));

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeNebl, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeNebl, TWHDVersionXPRV));

    assertStringsEqual(xpub, "xpub6D1srpSkopafPRy9JGYHWzoXcGtqcPQ3USgNDYJHY18YjZF7yh4pZLm3CsjcEN5imfjuRTtdFU9FvvMzFBoWHUeZTtrDDLs21qVQFBnJCrX");
    assertStringsEqual(xprv, "xprv9z2XTJuryT2NAwtgCF1H9rro4F4MCvgC7DkmR9tfyfbZrkuyS9ka1YSZMdKFQ47ztATXq6BHqXtaFsfUXux6rgZ6R9whhSej5n9Dh2KgrJB");
}

TEST(TWNeblAddress, DerivePubkeyFromXpub) {
    auto xpub = STRING("xpub6D1srpSkopafPRy9JGYHWzoXcGtqcPQ3USgNDYJHY18YjZF7yh4pZLm3CsjcEN5imfjuRTtdFU9FvvMzFBoWHUeZTtrDDLs21qVQFBnJCrX");
    auto pubKey3 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeNebl, STRING("m/44'/146'/0'/0/3").get()));
    auto pubKey5 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeNebl, STRING("m/44'/146'/0'/0/5").get()));

    auto address3 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubKey3.get(), TWCoinTypeNebl));
    auto address3String = WRAPS(TWAnyAddressDescription(address3.get()));

    auto address5 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubKey5.get(), TWCoinTypeNebl));
    auto address5String = WRAPS(TWAnyAddressDescription(address5.get()));

    assertStringsEqual(address3String, "NWeD8WdPtjrY1mM3zEmuA5xJyoVoNTgPus");
    assertStringsEqual(address5String, "NYCmtT7GtVweoLe4K1j975zuMpPQTyjsG3");
}

TEST(TWNeblAddress, DerivePubkeyFromXpub2) {
    auto xpub = STRING("xpub6D1srpSkopafPRy9JGYHWzoXcGtqcPQ3USgNDYJHY18YjZF7yh4pZLm3CsjcEN5imfjuRTtdFU9FvvMzFBoWHUeZTtrDDLs21qVQFBnJCrX");
    auto pubKey = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeNebl, STRING("m/44'/146'/0'/0/0").get()));
    auto address = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeNebl, pubKey.get()));
    assertStringsEqual(address, "NVeK3SbhgaTDRpQgnqi2Qv4xfzHKSyagod");
}

TEST(TWNeblAddress, LockScripts) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("NVeK3SbhgaTDRpQgnqi2Qv4xfzHKSyagod").get(), TWCoinTypeNebl));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a9146abb2d8e5858294d873383b108bcbb7da04edcea88ac");

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("NYCmtT7GtVweoLe4K1j975zuMpPQTyjsG3").get(), TWCoinTypeNebl));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "76a91486cf263c561daddea30f197691b7f66555d9620288ac");
}
