// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHDWallet.h>

#include "../HDWallet.h"

using namespace TW;

bool TWHDWalletIsValid(TWString *_Nonnull mnemonic) {
    return HDWallet::isValid(TWStringUTF8Bytes(mnemonic));
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

struct TWPrivateKey *_Nonnull TWHDWalletGetKey(struct TWHDWallet *wallet, TWPurpose purpose, TWCoinType coin, uint32_t account, uint32_t change, uint32_t address) {
    return new TWPrivateKey{ wallet->impl.getKey(purpose, coin, account, change, address) };
}

TWString *_Nonnull TWHDWalletGetExtendedPrivateKey(struct TWHDWallet *wallet, TWPurpose purpose, TWCoinType coin, TWHDVersion version) {
    return new std::string(wallet->impl.getExtendedPrivateKey(purpose, coin, version));
}

TWString *_Nonnull TWHDWalletGetExtendedPublicKey(struct TWHDWallet *wallet, TWPurpose purpose, TWCoinType coin, TWHDVersion version) {
    return new std::string(wallet->impl.getExtendedPublicKey(purpose, coin, version));
}

TWPublicKey TWHDWalletGetPublicKeyFromExtended(TWString *_Nonnull extended, enum TWHDVersion versionPublic, enum TWHDVersion versionPrivate, uint32_t change, uint32_t address) {
    auto publicKey = HDWallet::getPublicKeyFromExtended(*reinterpret_cast<const std::string*>(extended), versionPublic, versionPrivate, change, address);
    auto result = TWPublicKey();
    std::copy(publicKey.bytes.begin(), publicKey.bytes.end(), result.bytes);
    return result;
}

TWString* TWHDWalletGetAddressFromExtended(TWString *_Nonnull extended, TWCoinType coinType, uint32_t change, uint32_t address) {
    auto result = HDWallet::getAddressFromExtended(*reinterpret_cast<const std::string*>(extended), coinType, change, address);
    if (!result) {
        return nullptr;
    }
    return new std::string(*result);
}
