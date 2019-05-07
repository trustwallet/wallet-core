// Copyright © 2017-2019 Trust Wallet.
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

TW_EXPORT_STATIC_METHOD
struct TWPrivateKey *_Nonnull TWPrivateKeyCreate(void);

TW_EXPORT_STATIC_METHOD
struct TWPrivateKey *_Nullable TWPrivateKeyCreateWithData(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
struct TWPrivateKey *_Nullable TWPrivateKeyCreateCopy(struct TWPrivateKey *_Nonnull key);

TW_EXPORT_METHOD
void TWPrivateKeyDelete(struct TWPrivateKey *_Nonnull pk);

TW_EXPORT_STATIC_METHOD
bool TWPrivateKeyIsValid(TWData *_Nonnull data);

TW_EXPORT_PROPERTY
TWData *_Nonnull TWPrivateKeyData(struct TWPrivateKey *_Nonnull pk);

/// Returns the public key associated with this pirvate key.
TW_EXPORT_METHOD
struct TWPublicKey *_Nonnull TWPrivateKeyGetPublicKeySecp256k1(struct TWPrivateKey *_Nonnull pk, bool compressed);

/// Returns the public key associated with this pirvate key.
TW_EXPORT_METHOD
struct TWPublicKey *_Nonnull TWPrivateKeyGetPublicKeyNist256p1(struct TWPrivateKey *_Nonnull pk);

/// Returns the public key associated with this pirvate key.
TW_EXPORT_METHOD
struct TWPublicKey *_Nonnull TWPrivateKeyGetPublicKeyEd25519(struct TWPrivateKey *_Nonnull pk);

/// Returns the public key associated with this pirvate key.
TW_EXPORT_METHOD
struct TWPublicKey *_Nonnull TWPrivateKeyGetPublicKeyEd25519Blake2b(struct TWPrivateKey *_Nonnull pk);

/// Signs a digest using ECDSA and given curve.
TW_EXPORT_METHOD
TWData *_Nullable TWPrivateKeySign(struct TWPrivateKey *_Nonnull pk, TWData *_Nonnull digest, enum TWCurve curve);

/// Signs a digest using ECDSA and given curve. The result is encoded with DER.
TW_EXPORT_METHOD
TWData *_Nullable TWPrivateKeySignAsDER(struct TWPrivateKey *_Nonnull pk, TWData *_Nonnull digest, enum TWCurve curve);

/// Signs a digest using ECDSA and given curve, returns schnoor signature.
TW_EXPORT_METHOD
TWData *_Nullable TWPrivateKeySignSchnorr(struct TWPrivateKey *_Nonnull pk, TWData *_Nonnull message, enum TWCurve curve);

TW_EXTERN_C_END
