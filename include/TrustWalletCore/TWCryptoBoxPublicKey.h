// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Public key used in `crypto_box` cryptography.
TW_EXPORT_CLASS
struct TWCryptoBoxPublicKey;

/// Determines if the given public key is valid or not.
///
/// \param data *non-null* byte array.
/// \return true if the public key is valid, false otherwise.
TW_EXPORT_STATIC_METHOD
bool TWCryptoBoxPublicKeyIsValid(TWData* _Nonnull data);

/// Create a `crypto_box` public key with the given block of data.
///
/// \param data *non-null* byte array. Expected to have 32 bytes.
/// \note Should be deleted with \tw_crypto_box_public_key_delete.
/// \return Nullable pointer to Public Key.
TW_EXPORT_STATIC_METHOD
struct TWCryptoBoxPublicKey* _Nullable TWCryptoBoxPublicKeyCreateWithData(TWData* _Nonnull data);

/// Delete the given public key.
///
/// \param publicKey *non-null* pointer to public key.
TW_EXPORT_METHOD
void TWCryptoBoxPublicKeyDelete(struct TWCryptoBoxPublicKey* _Nonnull publicKey);

/// Returns the raw data of the given public-key.
///
/// \param publicKey *non-null* pointer to a public key.
/// \return C-compatible result with a C-compatible byte array.
TW_EXPORT_PROPERTY
TWData* _Nonnull TWCryptoBoxPublicKeyData(struct TWCryptoBoxPublicKey* _Nonnull publicKey);

TW_EXTERN_C_END
