// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Account.h"
#include "EncryptionParameters.h"
#include "Data.h"
#include "../HDWallet.h"

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWDerivation.h>
#include <TrustWalletCore/TWStoredKeyEncryption.h>
#include <nlohmann/json.hpp>

#include <optional>
#include <string>
#include <vector>

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
    EncryptedPayload payload;

    /// Active accounts.  Address should be unique.
    std::vector<Account> accounts;

    /// Create a new StoredKey, with the given name, mnemonic and password.
    /// @throws std::invalid_argument if mnemonic is invalid
    static StoredKey createWithMnemonic(const std::string& name, const Data& password, const std::string& mnemonic, TWStoredKeyEncryptionLevel encryptionLevel, TWStoredKeyEncryption encryption = TWStoredKeyEncryptionAes128Ctr);

    /// Create a new StoredKey, with the given name, mnemonic and password.
    /// @throws std::invalid_argument if mnemonic is invalid
    static StoredKey createWithMnemonicRandom(const std::string& name, const Data& password, TWStoredKeyEncryptionLevel encryptionLevel, TWStoredKeyEncryption encryption = TWStoredKeyEncryptionAes128Ctr);

    /// Create a new StoredKey, with the given name, mnemonic and password, and also add the default address for the given coin..
    /// @throws std::invalid_argument if mnemonic is invalid
    static StoredKey createWithMnemonicAddDefaultAddress(const std::string& name, const Data& password, const std::string& mnemonic, TWCoinType coin, TWStoredKeyEncryption encryption = TWStoredKeyEncryptionAes128Ctr);

    /// Create a new StoredKey, with the given name and private key.
    /// @throws std::invalid_argument if privateKeyData is not a valid private key
    static StoredKey createWithPrivateKey(const std::string& name, const Data& password, const Data& privateKeyData, TWStoredKeyEncryption encryption = TWStoredKeyEncryptionAes128Ctr);

    /// Create a new StoredKey, with the given name and private key, and also add the default address for the given coin..
    /// @throws std::invalid_argument if privateKeyData is not a valid private key
    static StoredKey createWithPrivateKeyAddDefaultAddress(const std::string& name, const Data& password, TWCoinType coin, const Data& privateKeyData, TWStoredKeyEncryption encryption = TWStoredKeyEncryptionAes128Ctr);

    /// Create a StoredKey from a JSON object.
    static StoredKey createWithJson(const nlohmann::json& json);

    /// Returns the HDWallet for this key.
    ///
    /// @throws std::invalid_argument if this key is of a type other than `mnemonicPhrase`.
    const HDWallet<> wallet(const Data& password) const;

    /// Returns all the accounts for a specific coin: 0, 1, or more.
    std::vector<Account> getAccounts(TWCoinType coin) const;

    /// If found, returns the account for a specific coin. In case of muliple accounts, the default derivation is returned, or the first one is returned.
    /// If none exists, and wallet is not null, an account is created (with default derivation).
    std::optional<const Account> account(TWCoinType coin, const HDWallet<>* wallet);

    /// If found, returns the account for a specific coin and derivation.  In case of muliple accounts, the first one is returned.
    /// If none exists, an account is created.
    Account account(TWCoinType coin, TWDerivation derivation, const HDWallet<>& wallet);

    /// Returns the account for a specific coin if it exists.
    /// In case of muliple accounts, the default derivation is returned, or the first one is returned.
    std::optional<const Account> account(TWCoinType coin) const;
    
    /// Returns the account for a specific coin and derivation, if it exists.
    std::optional<const Account> account(TWCoinType coin, TWDerivation derivation, const HDWallet<>& wallet) const;

    /// Add an account with aribitrary address/derivation path.  Discouraged, use account() versions.
    /// Address must be unique (for a coin).
    void addAccount(
        const std::string& address,
        TWCoinType coin,
        TWDerivation derivation,
        const DerivationPath& derivationPath,
        const std::string& publicKey,
        const std::string& extendedPublicKey
    );

    /// Remove the account(s) for a specific coin
    void removeAccount(TWCoinType coin);

    /// Remove the account for a specific coin with the given derivation.
    void removeAccount(TWCoinType coin, TWDerivation derivation);

    /// Remove the account for a specific coin with the given derivation path.
    void removeAccount(TWCoinType coin, DerivationPath derivationPath);

    /// Returns the private key for a specific coin, using default derivation, creating an account if necessary.
    ///
    /// \throws std::invalid_argument if this key is of a type other than
    /// `mnemonicPhrase` and a coin other than the default is requested.
    const PrivateKey privateKey(TWCoinType coin, const Data& password);

    /// Returns the private key for a specific coin, creating an account if necessary.
    ///
    /// \throws std::invalid_argument if this key is of a type other than
    /// `mnemonicPhrase` and a coin other than the default is requested.
    const PrivateKey privateKey(TWCoinType coin, TWDerivation derivation, const Data& password);

    /// Loads and decrypts a stored key from a file.
    ///
    /// \param path file path to load from.
    /// \returns decrypted key.
    /// \throws DecryptionError
    static StoredKey load(const std::string& path);

    /// Stores the key into an encrypted file.
    ///
    /// \param path file path to store in.
    void store(const std::string& path);

    /// Initializes `StoredKey` with a JSON object.
    void loadJson(const nlohmann::json& json);

    /// Saves `this` as a JSON object.
    nlohmann::json json() const;

    /// Fills in all empty or invalid addresses and public keys.
    ///
    /// Use to fix legacy wallets with invalid address data. This method needs
    /// the encryption password to re-derive addresses from private keys.
    void fixAddresses(const Data& password);

private:
    /// Default constructor, private
    StoredKey() : type(StoredKeyType::mnemonicPhrase) {}

    /// Initializes a `StoredKey` with a type, an encryption password, and unencrypted data.
    /// This constructor will encrypt the provided data with default encryption
    /// parameters.
    StoredKey(StoredKeyType type, std::string name, const Data& password, const Data& data, TWStoredKeyEncryptionLevel encryptionLevel, TWStoredKeyEncryption encryption = TWStoredKeyEncryptionAes128Ctr);

    /// Find default account for coin, if exists.  If multiple exist, default is returned.
    /// Optional wallet is needed to derive default address
    std::optional<Account> getDefaultAccount(TWCoinType coin, const HDWallet<>* wallet) const;

    /// Find account for coin, if exists.  If multiple exist, default is returned, or any.
    /// Optional wallet is needed to derive default address
    std::optional<Account> getDefaultAccountOrAny(TWCoinType coin, const HDWallet<>* wallet) const;

    /// Find account by coin+address (should be one, if multiple, first is returned)
    std::optional<Account> getAccount(TWCoinType coin, const std::string& address) const;

    /// Find account by coin+derivation (should be one, if multiple, first is returned)
    std::optional<Account> getAccount(TWCoinType coin, TWDerivation derivation, const HDWallet<>& wallet) const;

    /// Re-derive account address if missing
    Account fillAddressIfMissing(Account& account, const HDWallet<>* wallet) const;
};

} // namespace TW::Keystore

/// Wrapper for C interface.
struct TWStoredKey {
    TW::Keystore::StoredKey impl;
};
