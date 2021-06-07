// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHDWallet.h>

#include "../Coin.h"
#include "../HDWallet.h"
#include "../Mnemonic.h"

using namespace TW;

bool TWHDWalletIsValid(TWString *_Nonnull mnemonic) {
    return Mnemonic::isValid(TWStringUTF8Bytes(mnemonic));
}

struct TWHDWallet *_Nonnull TWHDWalletCreate(int strength, TWString *_Nonnull passphrase) {
    return new TWHDWallet{ HDWallet(strength, TWStringUTF8Bytes(passphrase)) };
}

struct TWHDWallet *_Nonnull TWHDWalletCreateWithMnemonic(TWString *_Nonnull mnemonic, TWString *_Nonnull passphrase) {
    return new TWHDWallet{ HDWallet(TWStringUTF8Bytes(mnemonic), TWStringUTF8Bytes(passphrase)) };
}

struct TWHDWallet *_Nonnull TWHDWalletCreateWithData(TWData *_Nonnull data, TWString *_Nonnull passphrase) {
    auto *d = reinterpret_cast<const Data*>(data);
    return new TWHDWallet{ HDWallet(*d, TWStringUTF8Bytes(passphrase)) };
}

void TWHDWalletDelete(struct TWHDWallet *wallet) {
    delete wallet;
}

TWData *_Nonnull TWHDWalletSeed(struct TWHDWallet *_Nonnull wallet) {
    return TWDataCreateWithBytes(wallet->impl.seed.data(), HDWallet::seedSize);
}

TWString *_Nonnull TWHDWalletMnemonic(struct TWHDWallet *_Nonnull wallet){
    return TWStringCreateWithUTF8Bytes(wallet->impl.mnemonic.c_str());
}

struct TWPrivateKey *_Nonnull TWHDWalletGetMasterKey(struct TWHDWallet *_Nonnull wallet, TWCurve curve) {
    return new TWPrivateKey{ wallet->impl.getMasterKey(curve) };
}

struct TWPrivateKey *_Nonnull TWHDWalletGetKeyForCoin(struct TWHDWallet *wallet, TWCoinType coin) {
    auto derivationPath = TW::derivationPath(coin);
    return new TWPrivateKey{ wallet->impl.getKey(coin, derivationPath) };
}

TWString *_Nonnull TWHDWalletGetAddressForCoin(struct TWHDWallet *wallet, TWCoinType coin) {
    auto derivationPath = TW::derivationPath(coin);
    PrivateKey privateKey = wallet->impl.getKey(coin, derivationPath);
    std::string address = deriveAddress(coin, privateKey);
    return TWStringCreateWithUTF8Bytes(address.c_str());
}

struct TWPrivateKey *_Nonnull TWHDWalletGetKey(struct TWHDWallet *_Nonnull wallet, enum TWCoinType coin, TWString *_Nonnull derivationPath) {
    auto& s = *reinterpret_cast<const std::string*>(derivationPath);
    const auto path = DerivationPath(s);
    return new TWPrivateKey{ wallet->impl.getKey(coin, path) };
}

struct TWPrivateKey *_Nonnull TWHDWalletGetDerivedKey(struct TWHDWallet *_Nonnull wallet, enum TWCoinType coin, uint32_t account, uint32_t change, uint32_t address) {
    const auto derivationPath = DerivationPath(TW::purpose(coin), TW::slip44Id(coin), account, change, address);
    return new TWPrivateKey{ wallet->impl.getKey(coin, derivationPath) };
}

TWString *_Nonnull TWHDWalletGetExtendedPrivateKey(struct TWHDWallet *wallet, TWPurpose purpose, TWCoinType coin, TWHDVersion version) {
    return new std::string(wallet->impl.getExtendedPrivateKey(purpose, coin, version));
}

TWString *_Nonnull TWHDWalletGetExtendedPublicKey(struct TWHDWallet *wallet, TWPurpose purpose, TWCoinType coin, TWHDVersion version) {
    return new std::string(wallet->impl.getExtendedPublicKey(purpose, coin, version));
}

TWPublicKey *TWHDWalletGetPublicKeyFromExtended(TWString *_Nonnull extended, enum TWCoinType coin, TWString *_Nonnull derivationPath) {
    const auto derivationPathObject = DerivationPath(*reinterpret_cast<const std::string*>(derivationPath));
    auto publicKey = HDWallet::getPublicKeyFromExtended(*reinterpret_cast<const std::string*>(extended), coin, derivationPathObject);
    if (!publicKey) {
        return nullptr;
    }
    return new TWPublicKey{ PublicKey(*publicKey) };
}
