// Copyright © 2017-2019 Trust Wallet.
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

TW_EXTERN_C_BEGIN

/// Represents a key stored as an encrypted file.
TW_EXPORT_CLASS
struct TWStoredKey;

/// Loads a key from a file.
TW_EXPORT_STATIC_METHOD
struct TWStoredKey *_Nullable TWStoredKeyLoad(TWString *_Nonnull path);

/// Imports a private key.
TW_EXPORT_STATIC_METHOD
struct TWStoredKey *_Nonnull TWStoredKeyImportPrivateKey(TWData *_Nonnull privateKey, TWString *_Nonnull password, enum TWCoinType coin);

/// Imports an HD wallet.
TW_EXPORT_STATIC_METHOD
struct TWStoredKey *_Nonnull TWStoredKeyImportHDWallet(TWString *_Nonnull mnemonic, TWString *_Nonnull password, enum TWCoinType coin);

/// Imports a key from JSON.
TW_EXPORT_STATIC_METHOD
struct TWStoredKey *_Nullable TWStoredKeyImportJSON(TWData *_Nonnull json);

/// Creates a new key.
TW_EXPORT_STATIC_METHOD
struct TWStoredKey *_Nonnull TWStoredKeyCreate(TWString *_Nonnull password);

TW_EXPORT_METHOD
void TWStoredKeyDelete(struct TWStoredKey *_Nonnull key);

/// Stored key uniqie identifier.
TW_EXPORT_PROPERTY
TWString *_Nullable TWStoredKeyIdentifier(struct TWStoredKey *_Nonnull key);

/// Whether this key is a mnemonic phrase for a HD wallet.
TW_EXPORT_PROPERTY
bool TWStoredKeyIsMnemonic(struct TWStoredKey *_Nonnull key);

/// The number of accounts.
TW_EXPORT_PROPERTY
size_t TWStoredKeyAccountCount(struct TWStoredKey *_Nonnull key);

/// Returns the account at a given index.
TW_EXPORT_METHOD
struct TWAccount *_Nullable TWStoredKeyAccount(struct TWStoredKey *_Nonnull key, size_t index);

/// Returns the account for a specific coin, creating it if necessary.
TW_EXPORT_METHOD
struct TWAccount *_Nullable TWStoredKeyAccountForCoin(struct TWStoredKey *_Nonnull key, enum TWCoinType coin, TWString *_Nonnull password);

/// Adds a new account.
TW_EXPORT_METHOD
void TWStoredKeyAddAccount(struct TWStoredKey *_Nonnull key, TWString *_Nonnull address, TWString *_Nonnull derivationPath, TWString *_Nonnull extetndedPublicKey);

/// Saves the key to a file.
TW_EXPORT_METHOD
bool TWStoredKeyStore(struct TWStoredKey *_Nonnull key, TWString *_Nonnull path);

/// Decrypts the private key.
TW_EXPORT_METHOD
TWData *_Nullable TWStoredKeyDecryptPrivateKey(struct TWStoredKey *_Nonnull key, TWString *_Nonnull password);

/// Decrypts the mnemonic phrase.
TW_EXPORT_METHOD
TWString *_Nullable TWStoredKeyDecryptMnemonic(struct TWStoredKey *_Nonnull key, TWString *_Nonnull password);

/// Returns the private key for a specific coin.
TW_EXPORT_METHOD
struct TWPrivateKey *_Nullable TWStoredKeyPrivateKey(struct TWStoredKey *_Nonnull key, enum TWCoinType coin, TWString *_Nonnull password);

/// Dercrypts and returns the HD Wallet for mnemonic phrase keys.
TW_EXPORT_METHOD
struct TWHDWallet *_Nullable TWStoredKeyWallet(struct TWStoredKey *_Nonnull key, TWString *_Nonnull password);

/// Exports the key as JSON
TW_EXPORT_METHOD
TWData *_Nullable TWStoredKeyExportJSON(struct TWStoredKey *_Nonnull key);

TW_EXTERN_C_END
