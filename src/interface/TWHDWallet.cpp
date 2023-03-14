// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHDWallet.h>

#include "../Coin.h"
#include "../HDWallet.h"
#include "../Mnemonic.h"

using namespace TW;


struct TWHDWallet *_Nullable TWHDWalletCreate(int strength, TWString *_Nonnull passphrase) {
    try {
        return new TWHDWallet{ HDWallet(strength, TWStringUTF8Bytes(passphrase)) };
    } catch (...) {
        return nullptr;
    }
}

struct TWHDWallet *_Nullable TWHDWalletCreateWithMnemonic(TWString *_Nonnull mnemonic, TWString *_Nonnull passphrase) {
    try {
        return new TWHDWallet{ HDWallet(TWStringUTF8Bytes(mnemonic), TWStringUTF8Bytes(passphrase)) };
    } catch (...) {
        return nullptr;
    }
}

struct TWHDWallet *_Nullable TWHDWalletCreateWithMnemonicCheck(TWString *_Nonnull mnemonic, TWString *_Nonnull passphrase, bool check) {
    try {
        return new TWHDWallet{ HDWallet(TWStringUTF8Bytes(mnemonic), TWStringUTF8Bytes(passphrase), check) };
    } catch (...) {
        return nullptr;
    }
}

struct TWHDWallet *_Nullable TWHDWalletCreateWithEntropy(TWData *_Nonnull entropy, TWString *_Nonnull passphrase) {
    try {
        auto* d = reinterpret_cast<const Data*>(entropy);
        return new TWHDWallet{ HDWallet(*d, TWStringUTF8Bytes(passphrase)) };
    } catch (...) {
        return nullptr;
    }
}

void TWHDWalletDelete(struct TWHDWallet *wallet) {
    delete wallet;
}

TWData *_Nonnull TWHDWalletSeed(struct TWHDWallet *_Nonnull wallet) {
    return TWDataCreateWithBytes(wallet->impl.getSeed().data(), HDWallet<>::mSeedSize);
}

TWString *_Nonnull TWHDWalletMnemonic(struct TWHDWallet *_Nonnull wallet){
    return TWStringCreateWithUTF8Bytes(wallet->impl.getMnemonic().c_str());
}

TWData *_Nonnull TWHDWalletEntropy(struct TWHDWallet *_Nonnull wallet) {
    return TWDataCreateWithBytes(wallet->impl.getEntropy().data(), wallet->impl.getEntropy().size());
}

struct TWPrivateKey *_Nonnull TWHDWalletGetMasterKey(struct TWHDWallet *_Nonnull wallet, TWCurve curve) {
    return new TWPrivateKey{ wallet->impl.getMasterKey(curve) };
}

struct TWPrivateKey *_Nonnull TWHDWalletGetKeyForCoin(struct TWHDWallet *wallet, TWCoinType coin) {
    return TWHDWalletGetKeyDerivation(wallet, coin, TWDerivationDefault);
}

TWString *_Nonnull TWHDWalletGetAddressForCoin(struct TWHDWallet *wallet, TWCoinType coin) {
    return TWHDWalletGetAddressDerivation(wallet, coin, TWDerivationDefault);
}

TWString *_Nonnull TWHDWalletGetAddressDerivation(struct TWHDWallet *wallet, TWCoinType coin, enum TWDerivation derivation) {
    auto derivationPath = TW::derivationPath(coin, derivation);
    PrivateKey privateKey = wallet->impl.getKey(coin, derivationPath);
    std::string address = deriveAddress(coin, privateKey, derivation);
    return TWStringCreateWithUTF8Bytes(address.c_str());
}

struct TWPrivateKey *_Nonnull TWHDWalletGetKey(struct TWHDWallet *_Nonnull wallet, enum TWCoinType coin, TWString *_Nonnull derivationPath) {
    auto& s = *reinterpret_cast<const std::string*>(derivationPath);
    const auto path = DerivationPath(s);
    return new TWPrivateKey{ wallet->impl.getKey(coin, path) };
}

struct TWPrivateKey *_Nonnull TWHDWalletGetKeyDerivation(struct TWHDWallet *_Nonnull wallet, enum TWCoinType coin, enum TWDerivation derivation) {
    auto derivationPath = TW::derivationPath(coin, derivation);
    return new TWPrivateKey{ wallet->impl.getKey(coin, derivationPath) };
}

struct TWPrivateKey *_Nonnull TWHDWalletGetDerivedKey(struct TWHDWallet *_Nonnull wallet, enum TWCoinType coin, uint32_t account, uint32_t change, uint32_t address) {
    const auto derivationPath = DerivationPath(TW::purpose(coin), TW::slip44Id(coin), account, change, address);
    return new TWPrivateKey{ wallet->impl.getKey(coin, derivationPath) };
}

struct TWPrivateKey *_Nonnull TWHDWalletGetKeyByCurve(struct TWHDWallet *_Nonnull wallet, enum TWCurve curve, TWString *_Nonnull derivationPath) {
    auto& s = *reinterpret_cast<const std::string*>(derivationPath);
    const auto path = DerivationPath(s);
    return new TWPrivateKey{ wallet->impl.getKeyByCurve(curve, path)};
}

TWString *_Nonnull TWHDWalletGetExtendedPrivateKey(struct TWHDWallet *wallet, TWPurpose purpose, TWCoinType coin, TWHDVersion version) {
    return new std::string(wallet->impl.getExtendedPrivateKey(purpose, coin, version));
}

TWString *_Nonnull TWHDWalletGetExtendedPublicKey(struct TWHDWallet *wallet, TWPurpose purpose, TWCoinType coin, TWHDVersion version) {
    return new std::string(wallet->impl.getExtendedPublicKey(purpose, coin, version));
}

TWString *_Nonnull TWHDWalletGetExtendedPrivateKeyAccount(struct TWHDWallet *wallet, TWPurpose purpose, TWCoinType coin, TWDerivation derivation, TWHDVersion version, uint32_t account) {
    return new std::string(wallet->impl.getExtendedPrivateKeyAccount(purpose, coin, derivation, version, account));
}

TWString *_Nonnull TWHDWalletGetExtendedPublicKeyAccount(struct TWHDWallet *wallet, TWPurpose purpose, TWCoinType coin, TWDerivation derivation, TWHDVersion version, uint32_t account) {
    return new std::string(wallet->impl.getExtendedPublicKeyAccount(purpose, coin, derivation, version, account));
}

TWString *_Nonnull TWHDWalletGetExtendedPrivateKeyDerivation(struct TWHDWallet *wallet, TWPurpose purpose, TWCoinType coin, TWDerivation derivation, TWHDVersion version) {
    return new std::string(wallet->impl.getExtendedPrivateKeyDerivation(purpose, coin, derivation, version));
}

TWString *_Nonnull TWHDWalletGetExtendedPublicKeyDerivation(struct TWHDWallet *wallet, TWPurpose purpose, TWCoinType coin, TWDerivation derivation, TWHDVersion version) {
    return new std::string(wallet->impl.getExtendedPublicKeyDerivation(purpose, coin, derivation, version));
}

TWPublicKey *TWHDWalletGetPublicKeyFromExtended(TWString *_Nonnull extended, enum TWCoinType coin, TWString *_Nonnull derivationPath) {
    const auto derivationPathObject = DerivationPath(*reinterpret_cast<const std::string*>(derivationPath));
    auto publicKey = HDWallet<>::getPublicKeyFromExtended(*reinterpret_cast<const std::string*>(extended), coin, derivationPathObject);
    if (!publicKey) {
        return nullptr;
    }
    return new TWPublicKey{ PublicKey(*publicKey) };
}
