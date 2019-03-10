// Copyright © 2017-2019 Trust Wallet.
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

struct TWStoredKey *_Nullable TWStoredKeyLoad(TWString *_Nonnull path, TWString *_Nonnull password) {
    auto& pathString = *reinterpret_cast<const std::string*>(path);
    auto& passwordString = *reinterpret_cast<const std::string*>(password);
    try {
        return new TWStoredKey{ StoredKey::load(pathString, passwordString) };
    } catch (std::exception) {
        return nullptr;
    }
}

struct TWStoredKey *_Nonnull TWStoredKeyCreate(TWString *_Nonnull password) {
    auto& passwordString = *reinterpret_cast<const std::string*>(password);
    auto mnemonic = TW::HDWallet(128, "");
    auto data = TW::Data(mnemonic.mnemonic.c_str(), mnemonic.mnemonic.c_str() + mnemonic.mnemonic.size());
    return new TWStoredKey{ StoredKey(StoredKeyType::mnemonicPhrase, passwordString, data) };
}

struct TWStoredKey *_Nullable TWStoredKeyImportPrivateKey(TWData *_Nonnull privateKey, TWString *_Nonnull password, enum TWCoinType coin) {
    auto& privateKeyData = *reinterpret_cast<const TW::Data*>(privateKey);
    auto& passwordString = *reinterpret_cast<const std::string*>(password);
    auto result = new TWStoredKey{ StoredKey(StoredKeyType::privateKey, passwordString, privateKeyData) };

    auto derivationPath = TW::derivationPath(coin);
    auto address = TW::deriveAddress(coin, TW::PrivateKey(privateKeyData));
    result->impl.accounts.emplace_back(address, derivationPath);

    return result;
}

struct TWStoredKey *_Nullable TWStoredKeyImportHDWallet(TWString *_Nonnull mnemonic, TWString *_Nonnull password, TWString *_Nonnull derivationPath) {
    auto& mnemonicString = *reinterpret_cast<const std::string*>(mnemonic);
    auto& passwordString = *reinterpret_cast<const std::string*>(password);
    auto& derivationPathString = *reinterpret_cast<const std::string*>(derivationPath);

    auto data = TW::Data(mnemonicString.c_str(), mnemonicString.c_str() + mnemonicString.size());
    auto result = new TWStoredKey{ StoredKey(StoredKeyType::mnemonicPhrase, passwordString, data) };

    auto wallet = TW::HDWallet(mnemonicString, "");
    auto dp = TW::DerivationPath(derivationPathString);
    auto address = TW::deriveAddress(dp.coin(), wallet.getKey(dp));
    result->impl.accounts.emplace_back(address, dp);

    return result;
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

size_t TWStoredKeyAccountCount(struct TWStoredKey *_Nonnull key) {
    return key->impl.accounts.size();
}

struct TWAccount *_Nullable TWStoredKeyAccount(struct TWStoredKey *_Nonnull key, size_t index) {
    if (index >= key->impl.accounts.size()) {
        return nullptr;
    }

    return new TWAccount{ key->impl.accounts[index] };
}

void TWStoredKeyAddAccount(struct TWStoredKey *_Nonnull key, TWString *_Nonnull address, TWString *_Nonnull derivationPath, TWString *_Nonnull extetndedPublicKey) {
    auto& addressString = *reinterpret_cast<const std::string*>(address);
    auto& extetndedPublicKeyString = *reinterpret_cast<const std::string*>(extetndedPublicKey);
    auto dp = TW::DerivationPath(*reinterpret_cast<const std::string*>(derivationPath));
    key->impl.accounts.emplace_back(addressString, dp, extetndedPublicKeyString);
}

bool TWStoredKeyStore(struct TWStoredKey *_Nonnull key, TWString *_Nonnull path, TWString *_Nonnull password) {
    auto& pathString = *reinterpret_cast<const std::string*>(path);
    auto& passwordString = *reinterpret_cast<const std::string*>(password);
    try {
        key->impl.store(pathString, passwordString);
    } catch (std::exception) {
        return false;
    }
    return true;
}

TWData *_Nonnull TWStoredKeyExportPrivateKey(struct TWStoredKey *_Nonnull key, TWString *_Nonnull password) {
    auto& passwordString = *reinterpret_cast<const std::string*>(password);
    const auto data = key->impl.payload.decrypt(passwordString);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWString *_Nonnull TWStoredKeyExportMnemonic(struct TWStoredKey *_Nonnull key, TWString *_Nonnull password) {
    auto& passwordString = *reinterpret_cast<const std::string*>(password);
    const auto data = key->impl.payload.decrypt(passwordString);
    return TWStringCreateWithUTF8Bytes(reinterpret_cast<const char*>(data.data()));
}
