// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWCryptoBoxPublicKey.h"
#include "TWCryptoBoxSecretKey.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// `crypto_box` encryption algorithms.
TW_EXPORT_STRUCT
struct TWCryptoBox;

/// Encrypts message using `my_secret` and `other_pubkey`.
/// The output will have a randomly generated nonce prepended to it.
/// The output will be Overhead + 24 bytes longer than the original.
///
/// \param mySecret *non-null* pointer to my secret key.
/// \param otherPubkey *non-null* pointer to other's public key.
/// \param message *non-null* pointer to the message to be encrypted.
/// \return *nullable* pointer to the encrypted message with randomly generated nonce prepended to it.
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWCryptoBoxEncryptEasy(struct TWCryptoBoxSecretKey* _Nonnull mySecret, struct TWCryptoBoxPublicKey* _Nonnull otherPubkey, TWData* _Nonnull message);

/// Decrypts box produced by `TWCryptoBoxEncryptEasy`.
/// We assume a 24-byte nonce is prepended to the encrypted text in box.
///
/// \param mySecret *non-null* pointer to my secret key.
/// \param otherPubkey *non-null* pointer to other's public key.
/// \param encrypted *non-null* pointer to the encrypted message with nonce prepended to it.
/// \return *nullable* pointer to the decrypted message.
TW_EXPORT_STATIC_METHOD
TWData* _Nullable TWCryptoBoxDecryptEasy(struct TWCryptoBoxSecretKey* _Nonnull mySecret, struct TWCryptoBoxPublicKey* _Nonnull otherPubkey, TWData* _Nonnull encrypted);

TW_EXTERN_C_END
