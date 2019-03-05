// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWPublicKeyEd25519.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWPrivateKeyEd25519;

static const size_t TWPrivateKeyEd25519Size = 32;

TW_EXPORT_STATIC_METHOD
struct TWPrivateKeyEd25519 *_Nonnull TWPrivateKeyEd25519Create(void);

TW_EXPORT_STATIC_METHOD
struct TWPrivateKeyEd25519 *_Nullable TWPrivateKeyEd25519CreateWithData(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
struct TWPrivateKeyEd25519 *_Nullable TWPrivateKeyEd25519CreateCopy(struct TWPrivateKeyEd25519 *_Nonnull key);

TW_EXPORT_METHOD
void TWPrivateKeyEd25519Delete(struct TWPrivateKeyEd25519 *_Nonnull pk);

TW_EXPORT_STATIC_METHOD
bool TWPrivateKeyEd25519IsValid(TWData *_Nonnull data);

TW_EXPORT_PROPERTY
TWData *_Nonnull TWPrivateKeyEd25519Data(struct TWPrivateKeyEd25519 *_Nonnull pk);

/// Returns the public key associated with this pirvate key.
TW_EXPORT_METHOD
struct TWPublicKeyEd25519 TWPrivateKeyEd25519GetPublicKey(struct TWPrivateKeyEd25519 *_Nonnull pk);

/// Signs a digest using Ed25519.
TW_EXPORT_METHOD
TWData *_Nullable TWPrivateKeyEd25519Sign(struct TWPrivateKeyEd25519 *_Nonnull pk, TWData *_Nonnull digest);

TW_EXTERN_C_END
