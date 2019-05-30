// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWP2PKHPrefix.h>
#include <TrustWalletCore/TWP2SHPrefix.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWPurpose.h>
#include <TrustWalletCore/TWBitcoinAddress.h>

#include <gtest/gtest.h>

TEST(MonetaryUnit, TransparentAddress) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("a22ddec5c567b4488bb00f69b6146c50da2ee883e2c096db098726394d585730").get()));
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true);
    auto address = TWBitcoinAddressCreateWithPublicKey(publicKey, TWP2PKHPrefixMonetaryUnit);
    auto addressString = WRAPS(TWBitcoinAddressDescription(address));
    assertStringsEqual(addressString, "7cJ2ye1qsjPCZgHuAiyDH62vHb2QsEj2BX");
}

TEST(MonetaryUnit, DeriveTransparentAddress) {
    auto words = STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal");
    auto passphrase = STRING("TREZOR");

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto derivationPath = STRING("m/44'/31'/0'/0/5");
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), derivationPath.get()));
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(key.get(), true);

    auto address = TWBitcoinAddressCreateWithPublicKey(publicKey, TWP2PKHPrefixMonetaryUnit);
    auto addressString = WRAPS(TWBitcoinAddressDescription(address));

    assertStringsEqual(addressString, "7nwu4fU9umfz4DajrPyQh3AQ8ESTywniZc");
}

TEST(MonetaryUnit, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeMonetaryUnit, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeMonetaryUnit, TWHDVersionXPRV));

    assertStringsEqual(xpub, "xpub6DKSaEADdUgknSNMW7hECg4GnDP1VQSprsJm5tWjHNupnJeW1LjjEyFy4WRg2zwPZFJFiSVNsyVqFE4uBv1y96tJzFmQo1h5BPZe8pUJkSY");
    assertStringsEqual(xprv, "xprv9zL6AidKo78TZxHtQ6ADqY7YEBYX5wiyVePAHW77j3NquWKMToRUhAwVDFiAQ1k2UgTeCyFMXTLjS7jUC52VoXiebkpbnZr8t3paNapL8cr");
}

TEST(MonetaryUnit, DerivePubkeyFromXpub) {
    auto xpub = STRING("xpub6DATuScKPEk6YvULrHPff1NKC49nyz5mCZQyxSDEQihq3kfoKDYCznLrsdW4KmXw9TryNfEZ9JSD8tJL9UTC3LnBA54YZL7nqMtJm7Ffnoz");
    auto pubKey3 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/31'/0'/0/3").get());
    auto pubKey5 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/31'/0'/0/5").get());

    auto address3 = TWBitcoinAddressCreateWithPublicKey(pubKey3, TWP2PKHPrefixMonetaryUnit);
    auto address3String = WRAPS(TWBitcoinAddressDescription(address3));

    auto address5 = TWBitcoinAddressCreateWithPublicKey(pubKey5, TWP2PKHPrefixMonetaryUnit);
    auto address5String = WRAPS(TWBitcoinAddressDescription(address5));

    assertStringsEqual(address3String, "7XAV4j6ev3fgfWY6rDLVXCHWRnsBBv4o5i");
    assertStringsEqual(address5String, "7cPunFisNtgjFKFAFk96mX54LFGtvozo3d");
}

TEST(MonetaryUnit, DerivePubkeyFromXpub2) {
    auto xpub = STRING("xpub6C7HhMqpir3KBA6ammv5B58RT3XFTJqoZFoj3J56dz9XwehZ2puSH38ERtnz7HaXGxaZP8AHT4M2bSRHpBXUZrbsJ2xg3xs53DGKYCqj8mr");
    auto pubKey = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/31'/0'/0/0").get());
    auto address = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeMonetaryUnit, pubKey));
    assertStringsEqual(address, "7brFdbavfxUb7TH8zJBcKu38Zshbeb1VJp");
}
