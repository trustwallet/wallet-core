// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWPublicKeySecp256k1.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWPrivateKeySecp256k1;

static const size_t TWPrivateKeySecp256k1Size = 32;

TW_EXPORT_STATIC_METHOD
struct TWPrivateKeySecp256k1 *_Nonnull TWPrivateKeySecp256k1Create(void);

TW_EXPORT_STATIC_METHOD
struct TWPrivateKeySecp256k1 *_Nullable TWPrivateKeySecp256k1CreateWithData(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
struct TWPrivateKeySecp256k1 *_Nullable TWPrivateKeySecp256k1CreateCopy(struct TWPrivateKeySecp256k1 *_Nonnull key);

TW_EXPORT_METHOD
void TWPrivateKeySecp256k1Delete(struct TWPrivateKeySecp256k1 *_Nonnull pk);

TW_EXPORT_STATIC_METHOD
bool TWPrivateKeySecp256k1IsValid(TWData *_Nonnull data);

TW_EXPORT_PROPERTY
TWData *_Nonnull TWPrivateKeySecp256k1Data(struct TWPrivateKeySecp256k1 *_Nonnull pk);

/// Returns the public key associated with this pirvate key.
TW_EXPORT_METHOD
struct TWPublicKeySecp256k1 TWPrivateKeySecp256k1GetPublicKey(struct TWPrivateKeySecp256k1 *_Nonnull pk, bool compressed);

/// Signs a digest using ECDSA secp256k1.
TW_EXPORT_METHOD
TWData *_Nullable TWPrivateKeySecp256k1Sign(struct TWPrivateKeySecp256k1 *_Nonnull pk, TWData *_Nonnull digest);

/// Signs a digest using ECDSA secp256k1. The result is encoded with DER.
TW_EXPORT_METHOD
TWData *_Nullable TWPrivateKeySecp256k1SignAsDER(struct TWPrivateKeySecp256k1 *_Nonnull pk, TWData *_Nonnull digest);

TW_EXTERN_C_END
