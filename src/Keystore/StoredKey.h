// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Account.h"
#include "EncryptionParameters.h"
#include "../Data.h"
#include "../HDWallet.h"

#include <nlohmann/json.hpp>
#include <TrustWalletCore/TWCoinType.h>

#include <optional>
#include <string>

namespace TW {
namespace Keystore {

/// An stored key can be either a private key or a mnemonic phrase for a HD wallet.
enum class StoredKeyType {
    privateKey,
    mnemonicPhrase,
    watchOnly
};

/// Represents a key stored as an encrypted file.
struct StoredKey {
    /// Type of key stored.
    StoredKeyType type;

    /// Encrypted payload.
    EncryptionParameters payload;

    /// Unique identifier.
    std::optional<std::string> id;

    /// Active accounts.
    std::vector<Account> accounts;

    /// Initializes a `StoredKey` with a type and an encrypted payload.
    StoredKey(StoredKeyType type, EncryptionParameters payload);

    /// Initializes a `StoredKey` with a type, an encryption password, and unencrypted data.
    ///
    /// This contstructor will encrypt the provided data with default encryption parameters.
    StoredKey(StoredKeyType type, const std::string& password, Data data);

    /// Returns the HDWallet for this key.
    ///
    /// @throws std::invalid_argument if this key is of a type other than `mnemonicPhrase`.
    HDWallet wallet(const std::string& password);

    /// Returns the account for a specific coin, creating it if necessary.
    ///
    /// @throws std::invalid_argument if this key is of a type other than `mnemonicPhrase` and an account
    /// other than the default is requested.
    const Account& account(TWCoinType coin, const std::string& password);

    /// Returns the private key for a specific coin, creating an account if necessary.
    ///
    /// @throws std::invalid_argument if this key is of a type other than `mnemonicPhrase` and a coin
    /// other than the default is requested.
    const PrivateKey privateKey(TWCoinType coin, const std::string& password);

    /// Loads and decrypts a stored key from a file.
    ///
    /// @param path file path to load from.
    /// @returns descrypted key.
    /// @throws DecryptionError
    static StoredKey load(const std::string& path);

    /// Stores the key into an encrypted file.
    ///
    /// @param path file path to store in.
    void store(const std::string& path);

    /// Initializes `StoredKey` with a JSON object.
    StoredKey(const nlohmann::json& json);

    /// Saves `this` as a JSON object.
    nlohmann::json json() const;
};

}} // namespace

/// Wrapper for C interface.
struct TWStoredKey {
    TW::Keystore::StoredKey impl;
};
