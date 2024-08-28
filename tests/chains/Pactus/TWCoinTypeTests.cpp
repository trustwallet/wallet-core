// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include "Pactus/Entry.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <gtest/gtest.h>

namespace TW::Pactus::tests {

TEST(TWPactusCoinType, TWCoinType) {
    const auto coin = TWCoinTypePactus;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes(""));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes(""));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "pactus");
    assertStringsEqual(name, "Pactus");
    assertStringsEqual(symbol, "PAC");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 9);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainPactus);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://pacviewer.com/transaction/");
    assertStringsEqual(accUrl, "https://pacviewer.com/address/");
}

TEST(TWPactusCoinType, DerivationPath) {
    auto derivationPath = TWCoinTypeDerivationPath(TWCoinTypePactus);
    assertStringsEqual(WRAPS(derivationPath), "m/44'/21888'/3'/0'");
}

TEST(TWPactusCoinType, HDWallet) {
    auto mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon cactus";
    auto passphrase = "";
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(STRING(mnemonic).get(), STRING(passphrase).get()));

    auto derivationPath1 = TWStringCreateWithUTF8Bytes("m/44'/21888'/3'/0'");
    auto privateKey1 = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), TWCoinTypePactus, derivationPath1));
    auto publicKey1 = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519(privateKey1.get()));
    auto address1 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey1.get(), TWCoinTypePactus));
    auto addressStr1 = WRAPS(TWAnyAddressDescription(address1.get()));

    auto derivationPath2 = TWStringCreateWithUTF8Bytes("m/44'/21888'/3'/1'");
    auto privateKey2 = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), TWCoinTypePactus, derivationPath2));
    auto publicKey2 = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519(privateKey2.get()));
    auto address2 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey2.get(), TWCoinTypePactus));
    auto addressStr2 = WRAPS(TWAnyAddressDescription(address2.get()));

    assertStringsEqual(addressStr1, "pc1rcx9x55nfme5juwdgxd2ksjdcmhvmvkrygmxpa3");
    assertStringsEqual(addressStr2, "pc1r7aynw9urvh66ktr3fte2gskjjnxzruflkgde94");
    TWStringDelete(derivationPath1);
    TWStringDelete(derivationPath2);
}

}