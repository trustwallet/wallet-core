// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWCurve.h"
#include "TWData.h"
#include "TWPublicKey.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWPrivateKey;

static const size_t TWPrivateKeySize = 32;

/// Create a random private key
///
/// \note Should be deleted with \TWPrivateKeyDelete
/// \return Non-null Private key
TW_EXPORT_STATIC_METHOD
struct TWPrivateKey* _Nonnull TWPrivateKeyCreate(void);

/// Create a private key with the given block of data
///
/// \param data a block of data
/// \note Should be deleted with \TWPrivateKeyDelete
/// \return Nullable pointer to Private Key
TW_EXPORT_STATIC_METHOD
struct TWPrivateKey* _Nullable TWPrivateKeyCreateWithData(TWData* _Nonnull data);

/// Deep copy a given private key
///
/// \param key Non-null private key to be copied
/// \note Should be deleted with \TWPrivateKeyDelete
/// \return Deep copy, Nullable pointer to Private key
TW_EXPORT_STATIC_METHOD
struct TWPrivateKey* _Nullable TWPrivateKeyCreateCopy(struct TWPrivateKey* _Nonnull key);

/// Delete the given private key
///
/// \param pk Non-null pointer to private key
TW_EXPORT_METHOD
void TWPrivateKeyDelete(struct TWPrivateKey* _Nonnull pk);

/// Determines if the given private key is valid or not.
///
/// \param data block of data (private key bytes)
/// \param curve Eliptic curve of the private key
/// \return true if the private key is valid, false otherwise
TW_EXPORT_STATIC_METHOD
bool TWPrivateKeyIsValid(TWData* _Nonnull data, enum TWCurve curve);

/// Convert the given private key to raw-bytes block of data
///
/// \param pk Non-null pointer to the private key
/// \return Non-null block of data (raw bytes) of the given private key
TW_EXPORT_PROPERTY
TWData* _Nonnull TWPrivateKeyData(struct TWPrivateKey* _Nonnull pk);

/// Returns the Secp256k1 public key associated with the given private key
///
/// \param pk Non-null pointer to the private key
/// \param compressed if the given private key is compressed or not
/// \return Non-null pointer to the corresponding public key
TW_EXPORT_METHOD
struct TWPublicKey* _Nonnull TWPrivateKeyGetPublicKeySecp256k1(struct TWPrivateKey* _Nonnull pk, bool compressed);

/// Returns the Nist256p1 public key associated with the given private key
///
/// \param pk Non-null pointer to the private key
/// \return Non-null pointer to the corresponding public key
TW_EXPORT_METHOD
struct TWPublicKey* _Nonnull TWPrivateKeyGetPublicKeyNist256p1(struct TWPrivateKey* _Nonnull pk);

/// Returns the Ed25519 public key associated with the given private key
///
/// \param pk Non-null pointer to the private key
/// \return Non-null pointer to the corresponding public key
TW_EXPORT_METHOD
struct TWPublicKey* _Nonnull TWPrivateKeyGetPublicKeyEd25519(struct TWPrivateKey* _Nonnull pk);

/// Returns the Ed25519Blake2b public key associated with the given private key
///
/// \param pk Non-null pointer to the private key
/// \return Non-null pointer to the corresponding public key
TW_EXPORT_METHOD
struct TWPublicKey* _Nonnull TWPrivateKeyGetPublicKeyEd25519Blake2b(struct TWPrivateKey* _Nonnull pk);

/// Returns the Ed25519Cardano public key associated with the given private key
///
/// \param pk Non-null pointer to the private key
/// \return Non-null pointer to the corresponding public key
TW_EXPORT_METHOD
struct TWPublicKey* _Nonnull TWPrivateKeyGetPublicKeyEd25519Cardano(struct TWPrivateKey* _Nonnull pk);

/// Returns the Curve25519 public key associated with the given private key
///
/// \param pk Non-null pointer to the private key
/// \return Non-null pointer to the corresponding public key
TW_EXPORT_METHOD
struct TWPublicKey* _Nonnull TWPrivateKeyGetPublicKeyCurve25519(struct TWPrivateKey* _Nonnull pk);

/// Computes an EC Diffie-Hellman secret in constant time
/// Supported curves: secp256k1
///
/// \param pk Non-null pointer to a Private key
/// \param publicKey Non-null pointer to the corresponding public key
/// \param curve Eliptic curve
/// \return The corresponding shared key as a non-null block of data
TW_EXPORT_METHOD
TWData* _Nullable TWPrivateKeyGetSharedKey(const struct TWPrivateKey* _Nonnull pk, const struct TWPublicKey* _Nonnull publicKey, enum TWCurve curve);

/// Signs a digest using ECDSA and given curve.
///
/// \param pk  Non-null pointer to a Private key
/// \param digest Non-null digest block of data
/// \param curve Eliptic curve
/// \return Signature as a Non-null block of data
TW_EXPORT_METHOD
TWData* _Nullable TWPrivateKeySign(struct TWPrivateKey* _Nonnull pk, TWData* _Nonnull digest, enum TWCurve curve);

/// Signs a digest using ECDSA. The result is encoded with DER.
///
/// \param pk  Non-null pointer to a Private key
/// \param digest Non-null digest block of data
/// \return Signature as a Non-null block of data
TW_EXPORT_METHOD
TWData* _Nullable TWPrivateKeySignAsDER(struct TWPrivateKey* _Nonnull pk, TWData* _Nonnull digest);

/// Signs a digest using ECDSA and Zilliqa schnorr signature scheme.
///
/// \param pk Non-null pointer to a Private key
/// \param message Non-null message
/// \return Signature as a Non-null block of data
TW_EXPORT_METHOD
TWData* _Nullable TWPrivateKeySignZilliqaSchnorr(struct TWPrivateKey* _Nonnull pk, TWData* _Nonnull message);

TW_EXTERN_C_END
