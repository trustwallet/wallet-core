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
#include <TrustWalletCore/TWPrivateKey.h>

#include <gtest/gtest.h>

TEST(MonetaryUnit, LegacyAddress) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("a22ddec5c567b4488bb00f69b6146c50da2ee883e2c096db098726394d585730").get()));
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true);
    auto address = TWBitcoinAddress();
    TWBitcoinAddressInitWithPublicKey(&address, publicKey, TWP2PKHPrefixMonetaryUnit);
    auto addressString = WRAPS(TWBitcoinAddressDescription(address));
    assertStringsEqual(addressString, "VjtD8cQgvesPYWxfWoHjwz1BuLCHwDn7PA");
}

TEST(MonetaryUnit, Address) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("55f9cbb0376c422946fa28397c1219933ac60b312ede41bfacaf701ecd546625").get()));
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true);
    auto address = WRAP(TWSegwitAddress, TWSegwitAddressCreateWithPublicKey(TWHRPMonetaryUnit, publicKey));
    auto string = WRAPS(TWSegwitAddressDescription(address.get()));

    assertStringsEqual(string, "via1qytnqzjknvv03jwfgrsmzt0ycmwqgl0asu2r3d2");
}

TEST(MonetaryUnit, BuildForAddressV) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("Vw6bJFaF5Hyiveko7dpqRjVvcTAsjz7eYa").get()));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a914e771c6695c5dd189ccc4ef00cd0f3db3096d79bd88ac");
}

TEST(MonetaryUnit, BuildForAddressE) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("ESxRxvhJP6ZKtYaMGjj48As1kgCh6hXa6X").get()));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "a9146b85b3dac9340f36b9d32bbacf2ffcb0851ef17987");
}

TEST(MonetaryUnit, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    // .bip44
    auto xpub = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeMonetaryUnit, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeMonetaryUnit, TWHDVersionXPRV));

    assertStringsEqual(xpub, "xpub6Cezzf2KCXiVS4fuL4REsdU7tKruYStmsSeFfR62Hui7DUgA45rVk9M8mwvvq9EK1k9riyaHgFUELk2GNpsubV2GxqsbDaiPQ1pxmWAgbLV");
    assertStringsEqual(xprv, "xprv9yfeb9VRNAACDabSE2tEWVXPLJ2R8zAvWDies2gQjaB8LgM1WYYFCM2evs7TNQxY2EZbnDoLKKNm4KzjAhYNTBovPtQGTSojmT3NrRLPXwy");

    // .bip49
    auto yprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP49, TWCoinTypeMonetaryUnit, TWHDVersionYPRV));
    auto ypub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP49, TWCoinTypeMonetaryUnit, TWHDVersionYPUB));

    assertStringsEqual(yprv, "yprvAKLdkDwhRv7XF5WTrvBfUJ24Fw9k1ucaAwapzJ9qf2wJJqThVpiQNbfUNy7ptmyBfPGy3BauBWHqoA33cox7decvLedUVe7d1GTHNe2u6pY");
    assertStringsEqual(ypub, "ypub6YKz9jUbGHfpTZavxwifqRxnoxzERNLRYAWRngZTDNUHBdnr3N2evPyxEDVzsHBv8ZCtwZ7Fdx9ge9zk4nheQiDVtEApy71r2m3sLkrjeVK");

    // .bip84
    auto zprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP84, TWCoinTypeMonetaryUnit, TWHDVersionZPRV));
    auto zpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP84, TWCoinTypeMonetaryUnit, TWHDVersionZPUB));
    assertStringsEqual(zprv, "zprvAcV8GDQ8b8UQpxtftFxgEbrftqsa5HxLjriVeMQEbvQFcKD7hsV5gY7DyLaGi448CNBKxZFspSKtiHNwd2VtqeUidEdD87SLumsmkDDvogA");
    assertStringsEqual(zpub, "zpub6qUUfiw2RW2i3Sy8zHVgbjoQSsi4UkgC75e6SjorAFwEV7YGFQoLELRhpe8NZ3xtsKHhbJW3kYWQLMFfCmMFDPkSer8BP9fiJcaZRL4LsR4");
}

TEST(MonetaryUnit, DeriveFromXpub) {
    auto xpub = STRING("xpub6BosfCnifzxcFwrSzQiqu2DBVTshkCXacvNsWGYJVVhhawA7d4R5WSWGFNbi8Aw6ZRc1brxMyWMzG3DSSSSoekkudhUd9yLb6qx39T9nMdj");
    auto pubKey2 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/14'/0'/0/2").get());
    auto pubKey9 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/14'/0'/0/9").get());

    TWBitcoinAddress address2;
    TWBitcoinAddressInitWithPublicKey(&address2, pubKey2, TWP2PKHPrefixMonetaryUnit);
    auto address2String = WRAPS(TWBitcoinAddressDescription(address2));

    TWBitcoinAddress address9;
    TWBitcoinAddressInitWithPublicKey(&address9, pubKey9, TWP2PKHPrefixMonetaryUnit);
    auto address9String = WRAPS(TWBitcoinAddressDescription(address9));

    assertStringsEqual(address2String, "VvN4z8c2zQA9gNnTTdxZkgYqagpVjkdb8z");
    assertStringsEqual(address9String, "VnUgk2EA8upaSFMfFwsT2kJbBKmWher7pC");
}

TEST(MonetaryUnit, DeriveFromZpub) {
    auto zpub = STRING("zpub6sCFp8chadVDXVt7GRmQFpq8B7W8wMLdFDto1hXu2jLZtvkFhRnwScXARNfrGSeyhR8DBLJnaUUkBbkmB2GwUYkecEAMUcbUpFQV4v7PXcs");
    auto pubKey4 = TWHDWalletGetPublicKeyFromExtended(zpub.get(), STRING("m/44'/14'/0'/0/4").get());
    auto pubKey11 = TWHDWalletGetPublicKeyFromExtended(zpub.get(), STRING("m/44'/14'/0'/0/11").get());

    auto address4 = WRAP(TWSegwitAddress, TWSegwitAddressCreateWithPublicKey(TWHRPMonetaryUnit, pubKey4));
    auto address4String = WRAPS(TWSegwitAddressDescription(address4.get()));

    auto address11 = WRAP(TWSegwitAddress, TWSegwitAddressCreateWithPublicKey(TWHRPMonetaryUnit, pubKey11));
    auto address11String = WRAPS(TWSegwitAddressDescription(address11.get()));

    assertStringsEqual(address4String, "7UQUGq9UQD57R5Kn6hHoYrpaRjDdWtZWF7");
    assertStringsEqual(address11String, "7oBSZsdN6jDoEn2agojLuUqoZbjWBbcwCm");
}

TEST(MonetaryUnit, LockScripts) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("7oBSZsdN6jDoEn2agojLuUqoZbjWBbcwCm").get()));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "001484542436f952c22c4c54a0fcd2c997c66317ea30");

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("ESxRxvhJP6ZKtYaMGjj48As1kgCh6hXa6X").get()));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "a9146b85b3dac9340f36b9d32bbacf2ffcb0851ef17987");

    auto script3 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("Vw6bJFaF5Hyiveko7dpqRjVvcTAsjz7eYa").get()));
    auto scriptData3 = WRAPD(TWBitcoinScriptData(script3.get()));
    assertHexEqual(scriptData3, "76a914e771c6695c5dd189ccc4ef00cd0f3db3096d79bd88ac");
}
