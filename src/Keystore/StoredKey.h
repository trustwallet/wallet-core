// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Account.h"
#include "EncryptionParameters.h"
#include "../Data.h"

#include <nlohmann/json.hpp>
#include <TrustWalletCore/TWCoinType.h>

#include <optional>
#include <string>

namespace TW {
namespace Keystore {

/// An stored key can be either a private key or a mnemonic phrase for a HD wallet.
enum class StoredKeyType {
    privateKey,
    mnemonicPhrase
};

/// Errors thrown when decrypting a key.
enum class DecryptionError {
    unsupportedKDF,
    unsupportedCipher,
    unsupportedCoin,
    invalidKeyFile,
    invalidCipher,
    invalidPassword,
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

    StoredKey(StoredKeyType type, Data payload) : type(type), payload(payload), id(), accounts() {}

    /// Loads and decrypts a stored key from a file.
    ///
    /// @param path file path to load from.
    /// @param password encryption password.
    /// @returns descrypted key.
    /// @throws DecryptionError
    static StoredKey load(const std::string& path, const std::string& password);

    /// Stores the key into an encrypted file.
    ///
    /// @param path file path to store in.
    /// @param password encryption password.
    void store(const std::string& path, const std::string& password);

    /// Initializes `StoredKey` with a JSON object.
    StoredKey(const nlohmann::json& json);

    /// Saves `this` as a JSON object.
    nlohmann::json json() const;
};

}} // namespace
