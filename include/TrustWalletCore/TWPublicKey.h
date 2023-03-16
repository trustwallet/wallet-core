// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWPublicKeyType.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

static const size_t TWPublicKeyCompressedSize = 33;
static const size_t TWPublicKeyUncompressedSize = 65;

/// Represents a public key.
TW_EXPORT_CLASS
struct TWPublicKey;

/// Create a public key from a block of data
///
/// \param data Non-null block of data representing the public key
/// \param type type of the public key
/// \note Should be deleted with \TWPublicKeyDelete
/// \return Nullable pointer to the public key
TW_EXPORT_STATIC_METHOD
struct TWPublicKey *_Nullable TWPublicKeyCreateWithData(TWData *_Nonnull data, enum TWPublicKeyType type);

/// Delete the given public key
///
/// \param pk Non-null pointer to a public key
TW_EXPORT_METHOD
void TWPublicKeyDelete(struct TWPublicKey *_Nonnull pk);

/// Determines if the given public key is valid or not
///
/// \param data Non-null block of data representing the public key
/// \param type type of the public key
/// \return true if the block of data is a valid public key, false otherwise
TW_EXPORT_STATIC_METHOD
bool TWPublicKeyIsValid(TWData *_Nonnull data, enum TWPublicKeyType type);

/// Determines if the given public key is compressed or not
///
/// \param pk Non-null pointer to a public key
/// \return true if the public key is compressed, false otherwise
TW_EXPORT_PROPERTY
bool TWPublicKeyIsCompressed(struct TWPublicKey *_Nonnull pk);

/// Give the compressed public key of the given non-compressed public key
///
/// \param from Non-null pointer to a non-compressed public key
/// \return Non-null pointer to the corresponding compressed public-key
TW_EXPORT_PROPERTY
struct TWPublicKey *_Nonnull TWPublicKeyCompressed(struct TWPublicKey *_Nonnull from);

/// Give the non-compressed public key of a corresponding compressed public key
///
/// \param from Non-null pointer to the corresponding compressed public key
/// \return Non-null pointer to the corresponding non-compressed public key
TW_EXPORT_PROPERTY
struct TWPublicKey *_Nonnull TWPublicKeyUncompressed(struct TWPublicKey *_Nonnull from);

/// Gives the raw data of a given public-key
///
/// \param pk Non-null pointer to a public key
/// \return Non-null pointer to the raw block of data of the given public key
TW_EXPORT_PROPERTY
TWData *_Nonnull TWPublicKeyData(struct TWPublicKey *_Nonnull pk);

/// Verify the validity of a signature and a message using the given public key
///
/// \param pk Non-null pointer to a public key
/// \param signature Non-null pointer to a block of data corresponding to the signature
/// \param message Non-null pointer to a block of data corresponding to the message
/// \return true if the signature and the message belongs to the given public key, false otherwise
TW_EXPORT_METHOD
bool TWPublicKeyVerify(struct TWPublicKey *_Nonnull pk, TWData *_Nonnull signature, TWData *_Nonnull message);

/// Verify the validity as DER of a signature and a message using the given public key
///
/// \param pk Non-null pointer to a public key
/// \param signature Non-null pointer to a block of data corresponding to the signature
/// \param message Non-null pointer to a block of data corresponding to the message
/// \return true if the signature and the message belongs to the given public key, false otherwise
TW_EXPORT_METHOD
bool TWPublicKeyVerifyAsDER(struct TWPublicKey *_Nonnull pk, TWData *_Nonnull signature, TWData *_Nonnull message);

/// Verify a Zilliqa schnorr signature with a signature and message.
///
/// \param pk Non-null pointer to a public key
/// \param signature Non-null pointer to a block of data corresponding to the signature
/// \param message Non-null pointer to a block of data corresponding to the message
/// \return true if the signature and the message belongs to the given public key, false otherwise
TW_EXPORT_METHOD
bool TWPublicKeyVerifyZilliqaSchnorr(struct TWPublicKey *_Nonnull pk, TWData *_Nonnull signature, TWData *_Nonnull message);

/// Give the public key type (eliptic) of a given public key
///
/// \param publicKey Non-null pointer to a public key
/// \return The public key type of the given public key (eliptic)
TW_EXPORT_PROPERTY
enum TWPublicKeyType TWPublicKeyKeyType(struct TWPublicKey *_Nonnull publicKey);

/// Get the public key description from a given public key
///
/// \param publicKey Non-null pointer to a public key
/// \return Non-null pointer to a string representing the description of the public key
TW_EXPORT_PROPERTY
TWString *_Nonnull TWPublicKeyDescription(struct TWPublicKey *_Nonnull publicKey);

/// Try to get a public key from a given signature and a message
///
/// \param signature Non-null pointer to a block of data corresponding to the signature
/// \param message Non-null pointer to a block of data corresponding to the message
/// \return Null pointer if the public key can't be recover from the given signature and message,
/// pointer to the public key otherwise
TW_EXPORT_STATIC_METHOD
struct TWPublicKey *_Nullable TWPublicKeyRecover(TWData *_Nonnull signature, TWData *_Nonnull message);

TW_EXTERN_C_END
