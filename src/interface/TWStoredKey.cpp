// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWStoredKey.h>

#include "../Coin.h"
#include "Data.h"
#include "../HDWallet.h"
#include "../Keystore/StoredKey.h"

#include <stdexcept>
#include <cassert>

namespace KeyStore = TW::Keystore;

struct TWStoredKey* _Nullable TWStoredKeyLoad(TWString* _Nonnull path) {
    try {
        const auto& pathString = *reinterpret_cast<const std::string*>(path);
        return new TWStoredKey{ KeyStore::StoredKey::load(pathString) };
    } catch (...) {
        return nullptr;
    }
}

struct TWStoredKey* _Nonnull TWStoredKeyCreateLevelAndEncryption(TWString* _Nonnull name, TWData* _Nonnull password, enum TWStoredKeyEncryptionLevel encryptionLevel, enum TWStoredKeyEncryption encryption) {
    const auto& nameString = *reinterpret_cast<const std::string*>(name);
    const auto passwordData = TW::data(TWDataBytes(password), TWDataSize(password));
    return new TWStoredKey{ KeyStore::StoredKey::createWithMnemonicRandom(nameString, passwordData, encryptionLevel, encryption) };
}

struct TWStoredKey* _Nonnull TWStoredKeyCreateLevel(TWString* _Nonnull name, TWData* _Nonnull password, enum TWStoredKeyEncryptionLevel encryptionLevel) {
   return TWStoredKeyCreateLevelAndEncryption(name, password, encryptionLevel, TWStoredKeyEncryptionAes128Ctr);
}

struct TWStoredKey* _Nonnull TWStoredKeyCreateEncryption(TWString* _Nonnull name, TWData* _Nonnull password, enum TWStoredKeyEncryption encryption) {
    return TWStoredKeyCreateLevelAndEncryption(name, password, TWStoredKeyEncryptionLevelDefault, encryption);
}

struct TWStoredKey* _Nonnull TWStoredKeyCreate(TWString* _Nonnull name, TWData* _Nonnull password) {
    return TWStoredKeyCreateLevelAndEncryption(name, password, TWStoredKeyEncryptionLevelDefault, TWStoredKeyEncryptionAes128Ctr);
}

struct TWStoredKey* _Nullable TWStoredKeyImportPrivateKey(TWData* _Nonnull privateKey, TWString* _Nonnull name, TWData* _Nonnull password, enum TWCoinType coin) {
    return TWStoredKeyImportPrivateKeyWithEncryption(privateKey, name, password, coin, TWStoredKeyEncryptionAes128Ctr);
}

struct TWStoredKey* _Nullable TWStoredKeyImportPrivateKeyWithEncryption(TWData* _Nonnull privateKey, TWString* _Nonnull name, TWData* _Nonnull password, enum TWCoinType coin, enum TWStoredKeyEncryption encryption) {
    try {
        const auto& privateKeyData = *reinterpret_cast<const TW::Data*>(privateKey);
        const auto& nameString = *reinterpret_cast<const std::string*>(name);
        const auto passwordData = TW::data(TWDataBytes(password), TWDataSize(password));
        return new TWStoredKey{ KeyStore::StoredKey::createWithPrivateKeyAddDefaultAddress(nameString, passwordData, coin, privateKeyData, encryption) };
    } catch (...) {
        return nullptr;
    }
}

struct TWStoredKey* _Nullable TWStoredKeyImportHDWallet(TWString* _Nonnull mnemonic, TWString* _Nonnull name, TWData* _Nonnull password, enum TWCoinType coin) {
    return TWStoredKeyImportHDWalletWithEncryption(mnemonic, name, password, coin, TWStoredKeyEncryptionAes128Ctr);
}


struct TWStoredKey* _Nullable TWStoredKeyImportHDWalletWithEncryption(TWString* _Nonnull mnemonic, TWString* _Nonnull name, TWData* _Nonnull password, enum TWCoinType coin, enum TWStoredKeyEncryption encryption) {
    try {
        const auto& mnemonicString = *reinterpret_cast<const std::string*>(mnemonic);
        const auto& nameString = *reinterpret_cast<const std::string*>(name);
        const auto passwordData = TW::data(TWDataBytes(password), TWDataSize(password));
        return new TWStoredKey{ KeyStore::StoredKey::createWithMnemonicAddDefaultAddress(nameString, passwordData, mnemonicString, coin, encryption) };
    } catch (...) {
        return nullptr;
    }
}

struct TWStoredKey* _Nullable TWStoredKeyImportJSON(TWData* _Nonnull json) {
    try {
        const auto& d = *reinterpret_cast<const TW::Data*>(json);
        const auto parsed = nlohmann::json::parse(d);
        return new TWStoredKey{ KeyStore::StoredKey::createWithJson(nlohmann::json::parse(d)) };
    } catch (...) {
        return nullptr;
    }
}

void TWStoredKeyDelete(struct TWStoredKey* _Nonnull key) {
    delete key;
}

TWString* _Nullable TWStoredKeyIdentifier(struct TWStoredKey* _Nonnull key) {
    if (!key->impl.id) {
        return nullptr;
    }
    return TWStringCreateWithUTF8Bytes(key->impl.id->c_str());
}

TWString* _Nonnull TWStoredKeyName(struct TWStoredKey* _Nonnull key) {
    return TWStringCreateWithUTF8Bytes(key->impl.name.c_str());
}

bool TWStoredKeyIsMnemonic(struct TWStoredKey* _Nonnull key) {
    return key->impl.type == KeyStore::StoredKeyType::mnemonicPhrase;
}

size_t TWStoredKeyAccountCount(struct TWStoredKey* _Nonnull key) {
    return key->impl.accounts.size();
}

struct TWAccount* _Nullable TWStoredKeyAccount(struct TWStoredKey* _Nonnull key, size_t index) {
    if (index >= key->impl.accounts.size()) {
        return nullptr;
    }

    return new TWAccount{ key->impl.accounts[index] };
}

struct TWAccount* _Nullable TWStoredKeyAccountForCoin(struct TWStoredKey* _Nonnull key, enum TWCoinType coin, struct TWHDWallet* _Nullable wallet) {
    try {
        const auto account = key->impl.account(coin, (wallet ? &wallet->impl : nullptr));
        // Note: std::optional.value() is not available in XCode with target < iOS 12, using *
        return (!account.has_value()) ? nullptr : new TWAccount{ *account };
    } catch (...) {
        return nullptr;
    }
}

struct TWAccount* _Nullable TWStoredKeyAccountForCoinDerivation(struct TWStoredKey* _Nonnull key, enum TWCoinType coin, TWDerivation derivation, struct TWHDWallet* _Nullable wallet) {
    try {
        if (wallet == nullptr) {
            return nullptr;
        }
        const auto account = key->impl.account(coin, derivation, wallet->impl);
        return new TWAccount{ account };
    } catch (...) {
        return nullptr;
    }
}

void TWStoredKeyRemoveAccountForCoin(struct TWStoredKey* _Nonnull key, enum TWCoinType coin) {
    key->impl.removeAccount(coin);
}

void TWStoredKeyRemoveAccountForCoinDerivation(struct TWStoredKey* _Nonnull key, enum TWCoinType coin, enum TWDerivation derivation) {
    key->impl.removeAccount(coin, derivation);
}

void TWStoredKeyRemoveAccountForCoinDerivationPath(struct TWStoredKey* _Nonnull key, enum TWCoinType coin, TWString* _Nonnull derivationPath) {
    const auto dp = TW::DerivationPath(*reinterpret_cast<const std::string*>(derivationPath));
    key->impl.removeAccount(coin, dp);
}

void TWStoredKeyAddAccountDerivation(struct TWStoredKey* _Nonnull key, TWString* _Nonnull address, enum TWCoinType coin, enum TWDerivation derivation, TWString* _Nonnull derivationPath, TWString* _Nonnull publicKey, TWString* _Nonnull extendedPublicKey) {
    const auto& addressString = *reinterpret_cast<const std::string*>(address);
    const auto& publicKeyString = *reinterpret_cast<const std::string*>(publicKey);
    const auto& extendedPublicKeyString = *reinterpret_cast<const std::string*>(extendedPublicKey);
    const auto dp = TW::DerivationPath(*reinterpret_cast<const std::string*>(derivationPath));
    key->impl.addAccount(addressString, coin, derivation, dp, publicKeyString, extendedPublicKeyString);
}

void TWStoredKeyAddAccount(struct TWStoredKey* _Nonnull key, TWString* _Nonnull address, enum TWCoinType coin, TWString* _Nonnull derivationPath, TWString* _Nonnull publicKey, TWString* _Nonnull extendedPublicKey) {
    return TWStoredKeyAddAccountDerivation(key, address, coin, TWDerivationDefault, derivationPath, publicKey, extendedPublicKey);
}

bool TWStoredKeyStore(struct TWStoredKey* _Nonnull key, TWString* _Nonnull path) {
    try {
        const auto& pathString = *reinterpret_cast<const std::string*>(path);
        key->impl.store(pathString);
        return true;
    } catch (...) {
        return false;
    }
}

TWData* _Nullable TWStoredKeyDecryptPrivateKey(struct TWStoredKey* _Nonnull key, TWData* _Nonnull password) {
    try {
        const auto passwordData = TW::data(TWDataBytes(password), TWDataSize(password));
        const auto data = key->impl.payload.decrypt(passwordData);
        return TWDataCreateWithBytes(data.data(), data.size());
    } catch (...) {
        return nullptr;
    }
}

TWString* _Nullable TWStoredKeyDecryptMnemonic(struct TWStoredKey* _Nonnull key, TWData* _Nonnull password) {
    try {
        const auto passwordData = TW::data(TWDataBytes(password), TWDataSize(password));
        const auto data = key->impl.payload.decrypt(passwordData);
        const auto string = std::string(data.begin(), data.end());
        return TWStringCreateWithUTF8Bytes(string.c_str());
    } catch (...) {
        return nullptr;
    }
}

struct TWPrivateKey* _Nullable TWStoredKeyPrivateKey(struct TWStoredKey* _Nonnull key, enum TWCoinType coin, TWData* _Nonnull password) {
    try {
        const auto passwordData = TW::data(TWDataBytes(password), TWDataSize(password));
        return new TWPrivateKey{ key->impl.privateKey(coin, passwordData) };
    } catch (...) {
        return nullptr;
    }
}

struct TWHDWallet* _Nullable TWStoredKeyWallet(struct TWStoredKey* _Nonnull key, TWData* _Nonnull password) {
    try {
        const auto passwordData = TW::data(TWDataBytes(password), TWDataSize(password));
        return new TWHDWallet{ key->impl.wallet(passwordData) };
    } catch (...) {
        return nullptr;
    }
}

TWData* _Nullable TWStoredKeyExportJSON(struct TWStoredKey* _Nonnull key) {
    const auto json = key->impl.json().dump();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(json.data()), json.size());
}

bool TWStoredKeyFixAddresses(struct TWStoredKey* _Nonnull key, TWData* _Nonnull password) {
    try {
        const auto passwordData = TW::data(TWDataBytes(password), TWDataSize(password));
        key->impl.fixAddresses(passwordData);
        return true;
    } catch (...) {
        return false;
    }
}

TWString* _Nullable TWStoredKeyEncryptionParameters(struct TWStoredKey* _Nonnull key) {
    if (!key->impl.id) {
        return nullptr;
    }
    const std::string params = key->impl.payload.json().dump();
    return TWStringCreateWithUTF8Bytes(params.c_str());
}
