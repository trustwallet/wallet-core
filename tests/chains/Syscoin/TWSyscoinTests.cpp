// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWBitcoinAddress.h>
#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWPrivateKey.h>

#include <gtest/gtest.h>

TEST(Syscoin, LegacyAddress) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("a22ddec5c567b4488bb00f69b6146c50da2ee883e2c096db098726394d585730").get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    auto address = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeP2pkhPrefix(TWCoinTypeSyscoin)));
    auto addressString = WRAPS(TWBitcoinAddressDescription(address.get()));
    assertStringsEqual(addressString, "SXBPFk2PFDAP13qyKSdC4yptsJ8kJRAT3T");
}

TEST(Syscoin, Address) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("55f9cbb0376c422946fa28397c1219933ac60b312ede41bfacaf701ecd546625").get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeSyscoin));
    auto string = WRAPS(TWAnyAddressDescription(address.get()));

    assertStringsEqual(string, "sys1qytnqzjknvv03jwfgrsmzt0ycmwqgl0as7lkcf7");
}

TEST(Syscoin, LockScriptForLegacyAddress) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("SXBPFk2PFDAP13qyKSdC4yptsJ8kJRAT3T").get(), TWCoinTypeSyscoin));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a9146c70e57df7b18eeb0198be9e254737ecd336ed8888ac");
}

TEST(Syscoin, LockScriptForAddress) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("sys1qytnqzjknvv03jwfgrsmzt0ycmwqgl0as7lkcf7").get(), TWCoinTypeSyscoin));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "001422e6014ad3631f1939281c3625bc98db808fbfb0");
}

TEST(Syscoin, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    // .bip44
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeSyscoin, TWHDVersionXPRV));
    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeSyscoin, TWHDVersionXPUB));

    assertStringsEqual(xprv, "xprv9yFNgN7z81uG6QtwFt7gvbmLeDGeGfS2ar3DunwEkZcC7uLBXyy4eaaV3ir769zMLe3eHuTaGUtWVXwp6dkunLsfmA7bf3XqEFpTjHxSijx");
    assertStringsEqual(xpub, "xpub6CEj5sesxPTZJtyQMuehHji5CF78g89sx4xpiBLrJu9AzhfL5XHKCNtxtzPzyGxqb6jMbZfmbHeSGZZArL4hLttmdC6KEMuiWy7VocTYjzR");

    // .bip49
    auto yprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP49, TWCoinTypeSyscoin, TWHDVersionYPRV));
    auto ypub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP49, TWCoinTypeSyscoin, TWHDVersionYPUB));

    assertStringsEqual(yprv, "yprvAJAofBFEEQ1DLJJVMkPr4pufHLUKZ2VSbtHqPpphEgwgfvG8exgadM8vtW8AW52N7tqU4qM8JHk5xZkq3icnzoph5QA5kRVHBnhXuRMGw2b");
    assertStringsEqual(ypub, "ypub6XAA4gn84mZWYnNxTmvrRxrPqNJoxVDHy7DSCDEJo2UfYibHCVzqB9TQjmL2TKSEZVFmTNcmdJXunEu6oV2AFQNeiszjzcnX4nbG27s4SgS");

    // .bip84
    auto zprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP84, TWCoinTypeSyscoin, TWHDVersionZPRV));
    auto zpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP84, TWCoinTypeSyscoin, TWHDVersionZPUB));
    assertStringsEqual(zprv, "zprvAcdCiLx9ooAFnC1hXh7stnobLnnu7u25rqfLeJ9v632xdCXJrc8KvgNk2eZeQQbPQHvcUpsfJzgyDkRdfnkT6vjpYqkxFv1LsPxQ7uFwLGy");
    assertStringsEqual(zpub, "zpub6qcZ7rV3eAiYzg6AdietFvkKtpdPXMjwE4awSgZXeNZwVzrTQ9SaUUhDswmdA4A5riyimx322es7niQvJ1Fbi3mJNSVz3d3f9GBsYBb8Wky");
}

TEST(Syscoin, DeriveFromZpub) {
    auto zpub = STRING("zpub6sCFp8chadVDXVt7GRmQFpq8B7W8wMLdFDto1hXu2jLZtvkFhRnwScXARNfrGSeyhR8DBLJnaUUkBbkmB2GwUYkecEAMUcbUpFQV4v7PXcs");
    auto pubKey4 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(zpub.get(), TWCoinTypeSyscoin, STRING("m/44'/2'/0'/0/4").get()));
    auto pubKey11 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(zpub.get(), TWCoinTypeSyscoin, STRING("m/44'/2'/0'/0/11").get()));

    auto address4 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubKey4.get(), TWCoinTypeSyscoin));
    auto address4String = WRAPS(TWAnyAddressDescription(address4.get()));

    auto address11 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubKey11.get(), TWCoinTypeSyscoin));
    auto address11String = WRAPS(TWAnyAddressDescription(address11.get()));

    assertStringsEqual(address4String, "sys1qcgnevr9rp7aazy62m4gen0tfzlssa52a2z04vc");
    assertStringsEqual(address11String, "sys1qy072y8968nzp6mz3j292h8lp72d678fchhmyta");
}
