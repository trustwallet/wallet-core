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

/// Create a random secret key.
///
/// \note Should be deleted with \tw_crypto_box_secret_key_delete.
/// \return *non-null* pointer to Secret Key.
TW_EXPORT_STATIC_METHOD
struct TWCryptoBoxSecretKey* _Nonnull TWCryptoBoxSecretKeyCreate();

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

TW_EXTERN_C_END
