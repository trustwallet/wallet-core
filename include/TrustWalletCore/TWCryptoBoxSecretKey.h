// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWCryptoBoxPublicKey.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Secret key used in `crypto_box` cryptography.
TW_EXPORT_CLASS
struct TWCryptoBoxSecretKey;

/// Determines if the given secret key is valid or not.
///
/// \param data *non-null* byte array.
/// \return true if the secret key is valid, false otherwise.
TW_EXPORT_STATIC_METHOD
bool TWCryptoBoxSecretKeyIsValid(TWData* _Nonnull data);

/// Create a random secret key.
///
/// \note Should be deleted with \tw_crypto_box_secret_key_delete.
/// \return *non-null* pointer to Secret Key.
TW_EXPORT_STATIC_METHOD
struct TWCryptoBoxSecretKey* _Nonnull TWCryptoBoxSecretKeyCreate();

/// Create a `crypto_box` secret key with the given block of data.
///
/// \param data *non-null* byte array. Expected to have 32 bytes.
/// \note Should be deleted with \tw_crypto_box_secret_key_delete.
/// \return Nullable pointer to Secret Key.
TW_EXPORT_STATIC_METHOD
struct TWCryptoBoxSecretKey* _Nullable TWCryptoBoxSecretKeyCreateWithData(TWData* _Nonnull data);

/// Delete the given secret `key`.
///
/// \param key *non-null* pointer to secret key.
TW_EXPORT_METHOD
void TWCryptoBoxSecretKeyDelete(struct TWCryptoBoxSecretKey* _Nonnull key);

/// Returns the public key associated with the given `key`.
///
/// \param key *non-null* pointer to the private key.
/// \return *non-null* pointer to the corresponding public key.
TW_EXPORT_METHOD
struct TWCryptoBoxPublicKey* _Nonnull TWCryptoBoxSecretKeyGetPublicKey(struct TWCryptoBoxSecretKey* _Nonnull key);

/// Returns the raw data of the given secret-key.
///
/// \param secretKey *non-null* pointer to a secret key.
/// \return C-compatible result with a C-compatible byte array.
TW_EXPORT_PROPERTY
TWData* _Nonnull TWCryptoBoxSecretKeyData(struct TWCryptoBoxSecretKey* _Nonnull secretKey);

TW_EXTERN_C_END
