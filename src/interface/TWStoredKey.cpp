// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWStoredKey.h>

#include "../Coin.h"
#include "../Data.h"
#include "../HDWallet.h"
#include "../Keystore/StoredKey.h"

#include <stdexcept>

using namespace TW::Keystore;

struct TWStoredKey *_Nullable TWStoredKeyLoad(TWString *_Nonnull path) {
    auto& pathString = *reinterpret_cast<const std::string*>(path);
    try {
        return new TWStoredKey{ StoredKey::load(pathString) };
    } catch (...) {
        return nullptr;
    }
}

struct TWStoredKey *_Nonnull TWStoredKeyCreate(TWString *_Nonnull name, TWString *_Nonnull password) {
    auto& nameString = *reinterpret_cast<const std::string*>(name);
    auto& passwordString = *reinterpret_cast<const std::string*>(password);
    auto mnemonic = TW::HDWallet(128, "");
    auto data = TW::Data(mnemonic.mnemonic.c_str(), mnemonic.mnemonic.c_str() + mnemonic.mnemonic.size());
    return new TWStoredKey{ StoredKey(StoredKeyType::mnemonicPhrase, nameString, passwordString, data) };
}

struct TWStoredKey *_Nullable TWStoredKeyImportPrivateKey(TWData *_Nonnull privateKey, TWString *_Nonnull name, TWString *_Nonnull password, enum TWCoinType coin) {
    auto& privateKeyData = *reinterpret_cast<const TW::Data*>(privateKey);
    auto curve = TW::curve(coin);
    if (!TW::PrivateKey::isValid(privateKeyData, curve)) {
        return nullptr;
    }

    auto& nameString = *reinterpret_cast<const std::string*>(name);
    auto& passwordString = *reinterpret_cast<const std::string*>(password);
    auto result = new TWStoredKey{ StoredKey(StoredKeyType::privateKey, nameString, passwordString, privateKeyData) };

    auto derivationPath = TW::derivationPath(coin);
    auto address = TW::deriveAddress(coin, TW::PrivateKey(privateKeyData));
    result->impl.accounts.emplace_back(address, derivationPath);

    return result;
}

struct TWStoredKey *_Nullable TWStoredKeyImportHDWallet(TWString *_Nonnull mnemonic, TWString *_Nonnull name, TWString *_Nonnull password, enum TWCoinType coin) {
    auto& mnemonicString = *reinterpret_cast<const std::string*>(mnemonic);
    if (!TW::HDWallet::isValid(mnemonicString)) {
        return nullptr;
    }

    auto& nameString = *reinterpret_cast<const std::string*>(name);
    auto& passwordString = *reinterpret_cast<const std::string*>(password);

    auto data = TW::Data(mnemonicString.c_str(), mnemonicString.c_str() + mnemonicString.size());
    auto result = new TWStoredKey{ StoredKey(StoredKeyType::mnemonicPhrase, nameString, passwordString, data) };

    auto wallet = TW::HDWallet(mnemonicString, "");
    const auto dp = TW::derivationPath(coin);
    const auto address = TW::deriveAddress(dp.coin(), wallet.getKey(dp));
    const auto extendedKey = wallet.getExtendedPublicKey(TW::purpose(coin), coin, TW::xpubVersion(coin));
    result->impl.accounts.emplace_back(address, dp, extendedKey);

    return result;
}

struct TWStoredKey *_Nullable TWStoredKeyImportJSON(TWData *_Nonnull json) {
    auto& d = *reinterpret_cast<const TW::Data*>(json);
    try {
        return new TWStoredKey{ StoredKey(nlohmann::json::parse(d)) };
    } catch (...) {
        return nullptr;
    }
}

void TWStoredKeyDelete(struct TWStoredKey *_Nonnull key) {
    delete key;
}

TWString *_Nullable TWStoredKeyIdentifier(struct TWStoredKey *_Nonnull key) {
    if (!key->impl.id) {
        return nullptr;
    }
    return TWStringCreateWithUTF8Bytes(key->impl.id->c_str());
}

TWString *_Nonnull TWStoredKeyName(struct TWStoredKey *_Nonnull key) {
    return TWStringCreateWithUTF8Bytes(key->impl.name.c_str());
}

bool TWStoredKeyIsMnemonic(struct TWStoredKey *_Nonnull key) {
    return key->impl.type == StoredKeyType::mnemonicPhrase;
}

size_t TWStoredKeyAccountCount(struct TWStoredKey *_Nonnull key) {
    return key->impl.accounts.size();
}

struct TWAccount *_Nullable TWStoredKeyAccount(struct TWStoredKey *_Nonnull key, size_t index) {
    if (index >= key->impl.accounts.size()) {
        return nullptr;
    }

    return new TWAccount{ key->impl.accounts[index] };
}

struct TWAccount *_Nullable TWStoredKeyAccountForCoin(struct TWStoredKey *_Nonnull key, enum TWCoinType coin, struct TWHDWallet *_Nullable wallet) {
    try {
        const auto account = key->impl.account(coin, (wallet ? &wallet->impl : nullptr));
        if (account == nullptr) {
            return nullptr;
        }
       return new TWAccount{ *account };
    } catch (...) {
        return nullptr;
    }
}

void TWStoredKeyRemoveAccountForCoin(struct TWStoredKey *_Nonnull key, enum TWCoinType coin) {
    key->impl.removeAccount(coin);
}

void TWStoredKeyAddAccount(struct TWStoredKey *_Nonnull key, TWString *_Nonnull address, TWString *_Nonnull derivationPath, TWString *_Nonnull extetndedPublicKey) {
    auto& addressString = *reinterpret_cast<const std::string*>(address);
    auto& extetndedPublicKeyString = *reinterpret_cast<const std::string*>(extetndedPublicKey);
    auto dp = TW::DerivationPath(*reinterpret_cast<const std::string*>(derivationPath));
    key->impl.accounts.emplace_back(addressString, dp, extetndedPublicKeyString);
}

bool TWStoredKeyStore(struct TWStoredKey *_Nonnull key, TWString *_Nonnull path) {
    auto& pathString = *reinterpret_cast<const std::string*>(path);
    try {
        key->impl.store(pathString);
    } catch (...) {
        return false;
    }
    return true;
}

TWData *_Nullable TWStoredKeyDecryptPrivateKey(struct TWStoredKey *_Nonnull key, TWString *_Nonnull password) {
    auto& passwordString = *reinterpret_cast<const std::string*>(password);
    try {
        const auto data = key->impl.payload.decrypt(passwordString);
        return TWDataCreateWithBytes(data.data(), data.size());
    } catch (...) {
        return nullptr;
    }
}

TWString *_Nullable TWStoredKeyDecryptMnemonic(struct TWStoredKey *_Nonnull key, TWString *_Nonnull password) {
    auto& passwordString = *reinterpret_cast<const std::string*>(password);
    try {
        const auto data = key->impl.payload.decrypt(passwordString);
        const auto string = std::string(data.begin(), data.end());
        return TWStringCreateWithUTF8Bytes(string.c_str());
    } catch (...) {
        return nullptr;
    }
}

struct TWPrivateKey *_Nullable TWStoredKeyPrivateKey(struct TWStoredKey *_Nonnull key, enum TWCoinType coin, TWString *_Nonnull password) {
    auto& passwordString = *reinterpret_cast<const std::string*>(password);
    try {
        return new TWPrivateKey{ key->impl.privateKey(coin, passwordString) };
    } catch (...) {
        return nullptr;
    }
}

struct TWHDWallet *_Nullable TWStoredKeyWallet(struct TWStoredKey *_Nonnull key, TWString *_Nonnull password) {
    auto& passwordString = *reinterpret_cast<const std::string*>(password);
    if (key->impl.type != StoredKeyType::mnemonicPhrase) {
        return nullptr;
    }
    try {
        return new TWHDWallet{ key->impl.wallet(passwordString) };
    } catch (...) {
        return nullptr;
    }
}

TWData *_Nullable TWStoredKeyExportJSON(struct TWStoredKey *_Nonnull key) {
    const auto json = key->impl.json().dump();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(json.data()), json.size());
}

bool TWStoredKeyFixAddresses(struct TWStoredKey *_Nonnull key, TWString *_Nonnull password) {
    auto& passwordString = *reinterpret_cast<const std::string*>(password);
    try {
        key->impl.fixAddresses(passwordString);
        return true;
    } catch (...) {
        return false;
    }
}
