// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"
#include "TWCoinType.h"

TW_EXTERN_C_BEGIN

/// Represents a key stored as an encrypted file.
TW_EXPORT_CLASS
struct TWStoredKey;

/// Loads a key from a file.
TW_EXPORT_STATIC_METHOD
struct TWStoredKey *_Nullable TWStoredKeyLoad(TWString *_Nonnull path, TWString *_Nonnull password);

/// Imports a private key.
TW_EXPORT_STATIC_METHOD
struct TWStoredKey *_Nullable TWStoredKeyImportPrivateKey(TWData *_Nonnull privateKey, TWString *_Nonnull password, enum TWCoinType coin);

/// Imports an HD wallett.
TW_EXPORT_STATIC_METHOD
struct TWStoredKey *_Nullable TWStoredKeyImportHDWallet(TWString *_Nonnull mnemonic, TWString *_Nonnull password, TWString *_Nonnull derivationPath);

/// Creates a new key.
TW_EXPORT_STATIC_METHOD
struct TWStoredKey *_Nonnull TWStoredKeyCreate(TWString *_Nonnull password);

TW_EXPORT_METHOD
void TWStoredKeyDelete(struct TWStoredKey *_Nonnull key);

/// Stored key uniqie identifier.
TW_EXPORT_PROPERTY
TWString *_Nullable TWStoredKeyIdentifier(struct TWStoredKey *_Nonnull key);

/// The number of accounts.
TW_EXPORT_PROPERTY
size_t TWStoredKeyAccountCount(struct TWStoredKey *_Nonnull key);

/// Returns the account at a given index.
TW_EXPORT_METHOD
struct TWAccount *_Nullable TWStoredKeyAccount(struct TWStoredKey *_Nonnull key, size_t index);

/// Adds a new account.
TW_EXPORT_METHOD
void TWStoredKeyAddAccount(struct TWStoredKey *_Nonnull key, TWString *_Nonnull address, TWString *_Nonnull derivationPath, TWString *_Nonnull extetndedPublicKey);

/// Saves the key to a file.
TW_EXPORT_METHOD
bool TWStoredKeyStore(struct TWStoredKey *_Nonnull key, TWString *_Nonnull path, TWString *_Nonnull password);

/// Exports a private key.
TW_EXPORT_METHOD
TWData *_Nonnull TWStoredKeyExportPrivateKey(struct TWStoredKey *_Nonnull key, TWString *_Nonnull password);

/// Exports a mnemonic phrase.
TW_EXPORT_METHOD
TWString *_Nonnull TWStoredKeyExportMnemonic(struct TWStoredKey *_Nonnull key, TWString *_Nonnull password);

TW_EXTERN_C_END
