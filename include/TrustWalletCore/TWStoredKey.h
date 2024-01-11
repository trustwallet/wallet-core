// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWCoinType.h"
#include "TWData.h"
#include "TWDerivation.h"
#include "TWHDWallet.h"
#include "TWPrivateKey.h"
#include "TWStoredKeyEncryptionLevel.h"
#include "TWStoredKeyEncryption.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Represents a key stored as an encrypted file.
TW_EXPORT_CLASS
struct TWStoredKey;

/// Loads a key from a file.
///
/// \param path filepath to the key as a non-null string
/// \note Returned object needs to be deleted with \TWStoredKeyDelete
/// \return Nullptr if the key can't be load, the stored key otherwise
TW_EXPORT_STATIC_METHOD
struct TWStoredKey* _Nullable TWStoredKeyLoad(TWString* _Nonnull path);

/// Imports a private key.
///
/// \param privateKey Non-null Block of data private key
/// \param name The name of the stored key to import as a non-null string
/// \param password Non-null block of data, password of the stored key
/// \param coin the coin type
/// \note Returned object needs to be deleted with \TWStoredKeyDelete
/// \return Nullptr if the key can't be imported, the stored key otherwise
TW_EXPORT_STATIC_METHOD
struct TWStoredKey* _Nullable TWStoredKeyImportPrivateKey(TWData* _Nonnull privateKey, TWString* _Nonnull name, TWData* _Nonnull password, enum TWCoinType coin);

/// Imports a private key.
///
/// \param privateKey Non-null Block of data private key
/// \param name The name of the stored key to import as a non-null string
/// \param password Non-null block of data, password of the stored key
/// \param coin the coin type
/// \param encryption cipher encryption mode
/// \note Returned object needs to be deleted with \TWStoredKeyDelete
/// \return Nullptr if the key can't be imported, the stored key otherwise
TW_EXPORT_STATIC_METHOD
struct TWStoredKey* _Nullable TWStoredKeyImportPrivateKeyWithEncryption(TWData* _Nonnull privateKey, TWString* _Nonnull name, TWData* _Nonnull password, enum TWCoinType coin, enum TWStoredKeyEncryption encryption);

/// Imports an HD wallet.
///
/// \param mnemonic Non-null bip39 mnemonic
/// \param name The name of the stored key to import as a non-null string
/// \param password Non-null block of data, password of the stored key
/// \param coin the coin type
/// \note Returned object needs to be deleted with \TWStoredKeyDelete
/// \return Nullptr if the key can't be imported, the stored key otherwise
TW_EXPORT_STATIC_METHOD
struct TWStoredKey* _Nullable TWStoredKeyImportHDWallet(TWString* _Nonnull mnemonic, TWString* _Nonnull name, TWData* _Nonnull password, enum TWCoinType coin);

/// Imports an HD wallet.
///
/// \param mnemonic Non-null bip39 mnemonic
/// \param name The name of the stored key to import as a non-null string
/// \param password Non-null block of data, password of the stored key
/// \param coin the coin type
/// \param encryption cipher encryption mode
/// \note Returned object needs to be deleted with \TWStoredKeyDelete
/// \return Nullptr if the key can't be imported, the stored key otherwise
TW_EXPORT_STATIC_METHOD
struct TWStoredKey* _Nullable TWStoredKeyImportHDWalletWithEncryption(TWString* _Nonnull mnemonic, TWString* _Nonnull name, TWData* _Nonnull password, enum TWCoinType coin, enum TWStoredKeyEncryption encryption);

/// Imports a key from JSON.
///
/// \param json Json stored key import format as a non-null block of data
/// \note Returned object needs to be deleted with \TWStoredKeyDelete
/// \return Nullptr if the key can't be imported, the stored key otherwise
TW_EXPORT_STATIC_METHOD
struct TWStoredKey* _Nullable TWStoredKeyImportJSON(TWData* _Nonnull json);

/// Creates a new key, with given encryption strength level. Returned object needs to be deleted.
///
/// \param name The name of the key to be stored
/// \param password Non-null block of data, password of the stored key
/// \param encryptionLevel The level of encryption, see \TWStoredKeyEncryptionLevel
/// \note Returned object needs to be deleted with \TWStoredKeyDelete
/// \return The stored key as a non-null pointer
TW_DEPRECATED_FOR("3.1.1", "TWStoredKeyCreateLevelAndEncryption")
TW_EXPORT_STATIC_METHOD
struct TWStoredKey* _Nonnull TWStoredKeyCreateLevel(TWString* _Nonnull name, TWData* _Nonnull password, enum TWStoredKeyEncryptionLevel encryptionLevel);

/// Creates a new key, with given encryption strength level.  Returned object needs to be deleted.
///
/// \param name The name of the key to be stored
/// \param password Non-null block of data, password of the stored key
/// \param encryptionLevel The level of encryption, see \TWStoredKeyEncryptionLevel
/// \param encryption cipher encryption mode
/// \note Returned object needs to be deleted with \TWStoredKeyDelete
/// \return The stored key as a non-null pointer
TW_EXPORT_STATIC_METHOD
struct TWStoredKey* _Nonnull TWStoredKeyCreateLevelAndEncryption(TWString* _Nonnull name, TWData* _Nonnull password, enum TWStoredKeyEncryptionLevel encryptionLevel, enum TWStoredKeyEncryption encryption);

/// Creates a new key.
///
/// \deprecated use TWStoredKeyCreateLevel.
/// \param name The name of the key to be stored
/// \param password Non-null block of data, password of the stored key
/// \note Returned object needs to be deleted with \TWStoredKeyDelete
/// \return The stored key as a non-null pointer
TW_EXPORT_STATIC_METHOD struct TWStoredKey* _Nonnull TWStoredKeyCreate(TWString* _Nonnull name, TWData* _Nonnull password);

/// Creates a new key.
///
/// \deprecated use TWStoredKeyCreateLevel.
/// \param name The name of the key to be stored
/// \param password Non-null block of data, password of the stored key
/// \param encryption cipher encryption mode
/// \note Returned object needs to be deleted with \TWStoredKeyDelete
/// \return The stored key as a non-null pointer
TW_EXPORT_STATIC_METHOD struct TWStoredKey* _Nonnull TWStoredKeyCreateEncryption(TWString* _Nonnull name, TWData* _Nonnull password, enum TWStoredKeyEncryption encryption);

/// Delete a stored key
///
/// \param key The key to be deleted
TW_EXPORT_METHOD
void TWStoredKeyDelete(struct TWStoredKey* _Nonnull key);

/// Stored key unique identifier.
///
/// \param key Non-null pointer to a stored key
/// \note Returned object needs to be deleted with \TWStringDelete
/// \return The stored key unique identifier if it's found, null pointer otherwise.
TW_EXPORT_PROPERTY
TWString* _Nullable TWStoredKeyIdentifier(struct TWStoredKey* _Nonnull key);

/// Stored key namer.
///
/// \param key Non-null pointer to a stored key
/// \note Returned object needs to be deleted with \TWStringDelete
/// \return The stored key name as a non-null string pointer.
TW_EXPORT_PROPERTY
TWString* _Nonnull TWStoredKeyName(struct TWStoredKey* _Nonnull key);

/// Whether this key is a mnemonic phrase for a HD wallet.
///
/// \param key Non-null pointer to a stored key
/// \return true if the given stored key is a mnemonic, false otherwise
TW_EXPORT_PROPERTY
bool TWStoredKeyIsMnemonic(struct TWStoredKey* _Nonnull key);

/// The number of accounts.
///
/// \param key Non-null pointer to a stored key
/// \return the number of accounts associated to the given stored key
TW_EXPORT_PROPERTY
size_t TWStoredKeyAccountCount(struct TWStoredKey* _Nonnull key);

/// Returns the account at a given index.
///
/// \param key Non-null pointer to a stored key
/// \param index the account index to be retrieved
/// \note Returned object needs to be deleted with \TWAccountDelete
/// \return Null pointer if the associated account is not found, pointer to the account otherwise.
TW_EXPORT_METHOD
struct TWAccount* _Nullable TWStoredKeyAccount(struct TWStoredKey* _Nonnull key, size_t index);

/// Returns the account for a specific coin, creating it if necessary.
///
/// \param key Non-null pointer to a stored key
/// \param coin The coin type
/// \param wallet The associated HD wallet, can be null.
/// \note Returned object needs to be deleted with \TWAccountDelete
/// \return Null pointer if the associated account is not found/not created, pointer to the account otherwise.
TW_EXPORT_METHOD
struct TWAccount* _Nullable TWStoredKeyAccountForCoin(struct TWStoredKey* _Nonnull key, enum TWCoinType coin, struct TWHDWallet* _Nullable wallet);

/// Returns the account for a specific coin + derivation, creating it if necessary.
///
/// \param key Non-null pointer to a stored key
/// \param coin The coin type
/// \param derivation The derivation for the given coin
/// \param wallet the associated HD wallet, can be null.
/// \note Returned object needs to be deleted with \TWAccountDelete
/// \return Null pointer if the associated account is not found/not created, pointer to the account otherwise.
TW_EXPORT_METHOD
struct TWAccount* _Nullable TWStoredKeyAccountForCoinDerivation(struct TWStoredKey* _Nonnull key, enum TWCoinType coin, enum TWDerivation derivation, struct TWHDWallet* _Nullable wallet);

/// Adds a new account, using given derivation (usually TWDerivationDefault)
/// and derivation path (usually matches path from derivation, but custom possible).
///
/// \param key Non-null pointer to a stored key
/// \param address Non-null pointer to the address of the coin for this account
/// \param coin coin type
/// \param derivation derivation of the given coin type
/// \param derivationPath HD bip44 derivation path of the given coin
/// \param publicKey Non-null public key of the given coin/address
/// \param extendedPublicKey Non-null extended public key of the given coin/address
TW_EXPORT_METHOD
void TWStoredKeyAddAccountDerivation(struct TWStoredKey* _Nonnull key, TWString* _Nonnull address, enum TWCoinType coin, enum TWDerivation derivation, TWString* _Nonnull derivationPath, TWString* _Nonnull publicKey, TWString* _Nonnull extendedPublicKey);

/// Adds a new account, using given derivation path.
///
/// \deprecated Use TWStoredKeyAddAccountDerivation (with TWDerivationDefault) instead.
/// \param key Non-null pointer to a stored key
/// \param address Non-null pointer to the address of the coin for this account
/// \param coin coin type
/// \param derivationPath HD bip44 derivation path of the given coin
/// \param publicKey Non-null public key of the given coin/address
/// \param extendedPublicKey Non-null extended public key of the given coin/address
TW_EXPORT_METHOD
void TWStoredKeyAddAccount(struct TWStoredKey* _Nonnull key, TWString* _Nonnull address, enum TWCoinType coin, TWString* _Nonnull derivationPath, TWString* _Nonnull publicKey, TWString* _Nonnull extendedPublicKey);

/// Remove the account for a specific coin
///
/// \param key Non-null pointer to a stored key
/// \param coin Account coin type to be removed
TW_EXPORT_METHOD
void TWStoredKeyRemoveAccountForCoin(struct TWStoredKey* _Nonnull key, enum TWCoinType coin);

/// Remove the account for a specific coin with the given derivation.
///
/// \param key Non-null pointer to a stored key
/// \param coin Account coin type to be removed
/// \param derivation The derivation of the given coin type
TW_EXPORT_METHOD
void TWStoredKeyRemoveAccountForCoinDerivation(struct TWStoredKey* _Nonnull key, enum TWCoinType coin, enum TWDerivation derivation);

/// Remove the account for a specific coin with the given derivation path.
///
/// \param key Non-null pointer to a stored key
/// \param coin Account coin type to be removed
/// \param derivationPath The derivation path (bip44) of the given coin type
TW_EXPORT_METHOD
void TWStoredKeyRemoveAccountForCoinDerivationPath(struct TWStoredKey* _Nonnull key, enum TWCoinType coin, TWString* _Nonnull derivationPath);

/// Saves the key to a file.
///
/// \param key Non-null pointer to a stored key
/// \param path Non-null string filepath where the key will be saved
/// \return true if the key was successfully stored in the given filepath file, false otherwise
TW_EXPORT_METHOD
bool TWStoredKeyStore(struct TWStoredKey* _Nonnull key, TWString* _Nonnull path);

/// Decrypts the private key.
///
/// \param key Non-null pointer to a stored key
/// \param password Non-null block of data, password of the stored key
/// \return Decrypted private key as a block of data if success, null pointer otherwise
TW_EXPORT_METHOD
TWData* _Nullable TWStoredKeyDecryptPrivateKey(struct TWStoredKey* _Nonnull key, TWData* _Nonnull password);

/// Decrypts the mnemonic phrase.
///
/// \param key Non-null pointer to a stored key
/// \param password Non-null block of data, password of the stored key
/// \return Bip39 decrypted mnemonic if success, null pointer otherwise
TW_EXPORT_METHOD
TWString* _Nullable TWStoredKeyDecryptMnemonic(struct TWStoredKey* _Nonnull key, TWData* _Nonnull password);

/// Returns the private key for a specific coin.  Returned object needs to be deleted.
///
/// \param key Non-null pointer to a stored key
/// \param coin Account coin type to be queried
/// \note Returned object needs to be deleted with \TWPrivateKeyDelete
/// \return Null pointer on failure, pointer to the private key otherwise
TW_EXPORT_METHOD
struct TWPrivateKey* _Nullable TWStoredKeyPrivateKey(struct TWStoredKey* _Nonnull key, enum TWCoinType coin, TWData* _Nonnull password);

/// Decrypts and returns the HD Wallet for mnemonic phrase keys.  Returned object needs to be deleted.
///
/// \param key Non-null pointer to a stored key
/// \param password Non-null block of data, password of the stored key
/// \note Returned object needs to be deleted with \TWHDWalletDelete
/// \return Null pointer on failure, pointer to the HDWallet otherwise
TW_EXPORT_METHOD
struct TWHDWallet* _Nullable TWStoredKeyWallet(struct TWStoredKey* _Nonnull key, TWData* _Nonnull password);

/// Exports the key as JSON
///
/// \param key Non-null pointer to a stored key
/// \return Null pointer on failure, pointer to a block of data containing the json otherwise
TW_EXPORT_METHOD
TWData* _Nullable TWStoredKeyExportJSON(struct TWStoredKey* _Nonnull key);

/// Fills in empty and invalid addresses.
/// This method needs the encryption password to re-derive addresses from private keys.
///
/// \param key Non-null pointer to a stored key
/// \param password Non-null block of data, password of the stored key
/// \return `false` if the password is incorrect, true otherwise.
TW_EXPORT_METHOD
bool TWStoredKeyFixAddresses(struct TWStoredKey* _Nonnull key, TWData* _Nonnull password);

/// Retrieve stored key encoding parameters, as JSON string.
///
/// \param key Non-null pointer to a stored key
/// \return Null pointer on failure, encoding parameter as a json string otherwise.
TW_EXPORT_PROPERTY
TWString* _Nullable TWStoredKeyEncryptionParameters(struct TWStoredKey* _Nonnull key);

TW_EXTERN_C_END
