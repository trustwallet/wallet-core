// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Account.h"
#include "EncryptionParameters.h"
#include "../Data.h"
#include "../HDWallet.h"

#include <TrustWalletCore/TWCoinType.h>
#include <nlohmann/json.hpp>

#include <optional>
#include <string>

namespace TW::Keystore {

/// An stored key can be either a private key or a mnemonic phrase for a HD
/// wallet.
enum class StoredKeyType { privateKey, mnemonicPhrase };

/// Represents a key stored as an encrypted file.
class StoredKey {
public:
    /// Type of key stored.
    StoredKeyType type;

    /// Unique identifier.
    std::optional<std::string> id;

    /// Name.
    std::string name;

    /// Encrypted payload.
    EncryptionParameters payload;

    /// Active accounts.
    std::vector<Account> accounts;

    /// Create a new StoredKey, with the given name, mnemonic and password.
    /// @throws std::invalid_argument if mnemonic is invalid
    static StoredKey createWithMnemonic(const std::string& name, const Data& password, const std::string& mnemonic);

    /// Create a new StoredKey, with the given name, mnemonic and password.
    /// @throws std::invalid_argument if mnemonic is invalid
    static StoredKey createWithMnemonicRandom(const std::string& name, const Data& password);

    /// Create a new StoredKey, with the given name, mnemonic and password, and also add the default address for the given coin..
    /// @throws std::invalid_argument if mnemonic is invalid
    static StoredKey createWithMnemonicAddDefaultAddress(const std::string& name, const Data& password, const std::string& mnemonic, TWCoinType coin);

    /// Create a new StoredKey, with the given name and private key.
    /// @throws std::invalid_argument if privateKeyData is not a vald private key
    static StoredKey createWithPrivateKey(const std::string& name, const Data& password, const Data& privateKeyData);

    /// Create a new StoredKey, with the given name and private key, and also add the default address for the given coin..
    /// @throws std::invalid_argument if privateKeyData is not a vald private key
    static StoredKey createWithPrivateKeyAddDefaultAddress(const std::string& name, const Data& password, TWCoinType coin, const Data& privateKeyData);

    /// Create a StoredKey from a JSON object.
    static StoredKey createWithJson(const nlohmann::json& json);

    /// Returns the HDWallet for this key.
    ///
    /// @throws std::invalid_argument if this key is of a type other than `mnemonicPhrase`.
    const HDWallet wallet(const Data& password) const;

    /// Returns the account for a specific coin, creating it if necessary and
    /// the provided wallet is not `nullptr`.
    std::optional<const Account> account(TWCoinType coin, const HDWallet* wallet);

    /// Returns the account for a specific coin if it exists.
    std::optional<const Account> account(TWCoinType coin) const;
    
    /// Add an account
    void addAccount(const std::string& address, TWCoinType coin, const DerivationPath& derivationPath, const std::string& extetndedPublicKey);

    /// Remove the account for a specific coin
    void removeAccount(TWCoinType coin);
    
    /// Returns the private key for a specific coin, creating an account if necessary.
    ///
    /// @throws std::invalid_argument if this key is of a type other than
    /// `mnemonicPhrase` and a coin other than the default is requested.
    const PrivateKey privateKey(TWCoinType coin, const Data& password);

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
    void loadJson(const nlohmann::json& json);

    /// Saves `this` as a JSON object.
    nlohmann::json json() const;

    /// Fills in all empty and invalid addresses.
    ///
    /// Use to fix legacy wallets with invalid address data. This method needs
    /// the encryption password to re-derive addresses from private keys.
    void fixAddresses(const Data& password);

private:
    /// Default constructor, private
    StoredKey() : type(StoredKeyType::mnemonicPhrase) {}

    /// Initializes a `StoredKey` with a type, an encryption password, and unencrypted data.
    /// This contstructor will encrypt the provided data with default encryption
    /// parameters.
    StoredKey(StoredKeyType type, std::string name, const Data& password, const Data& data);
};

} // namespace TW::Keystore

/// Wrapper for C interface.
struct TWStoredKey {
    TW::Keystore::StoredKey impl;
};
