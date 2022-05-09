// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWCoinType.h"
#include "TWData.h"
#include "TWHDWallet.h"
#include "TWPrivateKey.h"
#include "TWString.h"
#include "TWStoredKeyEncryptionLevel.h"
#include "TWDerivation.h"

TW_EXTERN_C_BEGIN

/// Represents a key stored as an encrypted file.
TW_EXPORT_CLASS
struct TWStoredKey;

/// Loads a key from a file.  Returned object needs to be deleted.
TW_EXPORT_STATIC_METHOD
struct TWStoredKey* _Nullable TWStoredKeyLoad(TWString* _Nonnull path);

/// Imports a private key.  Returned object needs to be deleted.
TW_EXPORT_STATIC_METHOD
struct TWStoredKey* _Nullable TWStoredKeyImportPrivateKey(TWData* _Nonnull privateKey, TWString* _Nonnull name, TWData* _Nonnull password, enum TWCoinType coin);

/// Imports an HD wallet.  Returned object needs to be deleted.
TW_EXPORT_STATIC_METHOD
struct TWStoredKey* _Nullable TWStoredKeyImportHDWallet(TWString* _Nonnull mnemonic, TWString* _Nonnull name, TWData* _Nonnull password, enum TWCoinType coin);

/// Imports a key from JSON.  Returned object needs to be deleted.
TW_EXPORT_STATIC_METHOD
struct TWStoredKey* _Nullable TWStoredKeyImportJSON(TWData* _Nonnull json);

/// Creates a new key, with given encryption strength level.  Returned object needs to be deleted.
TW_EXPORT_STATIC_METHOD
struct TWStoredKey* _Nonnull TWStoredKeyCreateLevel(TWString* _Nonnull name, TWData* _Nonnull password, enum TWStoredKeyEncryptionLevel encryptionLevel);

/// DEPRECATED, use TWStoredKeyCreateLevel. Creates a new key.  Returned object needs to be deleted.
TW_EXPORT_STATIC_METHOD
struct TWStoredKey* _Nonnull TWStoredKeyCreate(TWString* _Nonnull name, TWData* _Nonnull password);

TW_EXPORT_METHOD
void TWStoredKeyDelete(struct TWStoredKey* _Nonnull key);

/// Stored key unique identifier.  Returned object needs to be deleted.
TW_EXPORT_PROPERTY
TWString* _Nullable TWStoredKeyIdentifier(struct TWStoredKey* _Nonnull key);

/// Stored key namer.  Returned object needs to be deleted.
TW_EXPORT_PROPERTY
TWString* _Nonnull TWStoredKeyName(struct TWStoredKey* _Nonnull key);

/// Whether this key is a mnemonic phrase for a HD wallet.
TW_EXPORT_PROPERTY
bool TWStoredKeyIsMnemonic(struct TWStoredKey* _Nonnull key);

/// The number of accounts.
TW_EXPORT_PROPERTY
size_t TWStoredKeyAccountCount(struct TWStoredKey* _Nonnull key);

/// Returns the account at a given index.  Returned object needs to be deleted.
TW_EXPORT_METHOD
struct TWAccount* _Nullable TWStoredKeyAccount(struct TWStoredKey* _Nonnull key, size_t index);

/// Returns the account for a specific coin, creating it if necessary.  Returned object needs to be deleted.
TW_EXPORT_METHOD
struct TWAccount* _Nullable TWStoredKeyAccountForCoin(struct TWStoredKey* _Nonnull key, enum TWCoinType coin, struct TWHDWallet* _Nullable wallet);

/// Returns the account for a specific coin + derivation, creating it if necessary.  Returned object needs to be deleted.
TW_EXPORT_METHOD
struct TWAccount* _Nullable TWStoredKeyAccountForCoinDerivation(struct TWStoredKey* _Nonnull key, enum TWCoinType coin, enum TWDerivation derivation, struct TWHDWallet* _Nullable wallet);

/// Adds a new account, using given derivation (usually TWDerivationDefault) and derivation path (usually matches path from derivation, but custom possible).
TW_EXPORT_METHOD
void TWStoredKeyAddAccountDerivation(struct TWStoredKey* _Nonnull key, TWString* _Nonnull address, enum TWCoinType coin, enum TWDerivation derivation, TWString* _Nonnull derivationPath, TWString* _Nonnull publicKey, TWString* _Nonnull extendedPublicKey);

/// [Deprecated] Use TWStoredKeyAddAccountDerivation (with TWDerivationDefault) instead.
/// Adds a new account, using given derivation path.
TW_EXPORT_METHOD
void TWStoredKeyAddAccount(struct TWStoredKey* _Nonnull key, TWString* _Nonnull address, enum TWCoinType coin, TWString* _Nonnull derivationPath, TWString* _Nonnull publicKey, TWString* _Nonnull extendedPublicKey);

/// Remove the account for a specific coin
TW_EXPORT_METHOD
void TWStoredKeyRemoveAccountForCoin(struct TWStoredKey* _Nonnull key, enum TWCoinType coin);

/// Remove the account for a specific coin with the given derivation.
TW_EXPORT_METHOD
void TWStoredKeyRemoveAccountForCoinDerivation(struct TWStoredKey* _Nonnull key, enum TWCoinType coin, enum TWDerivation derivation);

/// Remove the account for a specific coin with the given derivation path.
TW_EXPORT_METHOD
void TWStoredKeyRemoveAccountForCoinDerivationPath(struct TWStoredKey* _Nonnull key, enum TWCoinType coin, TWString* _Nonnull derivationPath);

/// Saves the key to a file.
TW_EXPORT_METHOD
bool TWStoredKeyStore(struct TWStoredKey* _Nonnull key, TWString* _Nonnull path);

/// Decrypts the private key.
TW_EXPORT_METHOD
TWData* _Nullable TWStoredKeyDecryptPrivateKey(struct TWStoredKey* _Nonnull key, TWData* _Nonnull password);

/// Decrypts the mnemonic phrase.
TW_EXPORT_METHOD
TWString* _Nullable TWStoredKeyDecryptMnemonic(struct TWStoredKey* _Nonnull key, TWData* _Nonnull password);

/// Returns the private key for a specific coin.  Returned object needs to be deleted.
TW_EXPORT_METHOD
struct TWPrivateKey* _Nullable TWStoredKeyPrivateKey(struct TWStoredKey* _Nonnull key, enum TWCoinType coin, TWData* _Nonnull password);

/// Decrypts and returns the HD Wallet for mnemonic phrase keys.  Returned object needs to be deleted.
TW_EXPORT_METHOD
struct TWHDWallet* _Nullable TWStoredKeyWallet(struct TWStoredKey* _Nonnull key, TWData* _Nonnull password);

/// Exports the key as JSON
TW_EXPORT_METHOD
TWData* _Nullable TWStoredKeyExportJSON(struct TWStoredKey* _Nonnull key);

/// Fills in empty and invalid addresses.
///
/// This method needs the encryption password to re-derive addresses from private keys.
/// @returns `false` if the password is incorrect.
TW_EXPORT_METHOD
bool TWStoredKeyFixAddresses(struct TWStoredKey* _Nonnull key, TWData* _Nonnull password);

/// Retrieve stored key encoding parameters, as JSON string.
TW_EXPORT_PROPERTY
TWString* _Nullable TWStoredKeyEncryptionParameters(struct TWStoredKey* _Nonnull key);

TW_EXTERN_C_END
