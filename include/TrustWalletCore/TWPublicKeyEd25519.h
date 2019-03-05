// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

static const size_t TWPublicKeyEd25519Size = 32;

TW_EXPORT_STRUCT
struct TWPublicKeyEd25519 {
    uint8_t bytes[TWPublicKeyEd25519Size];
};

TW_EXPORT_STATIC_METHOD
bool TWPublicKeyEd25519InitWithData(struct TWPublicKeyEd25519 *_Nonnull pk, TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
bool TWPublicKeyEd25519IsValid(TWData *_Nonnull data);

TW_EXPORT_PROPERTY
TWData *_Nonnull TWPublicKeyEd25519Data(struct TWPublicKeyEd25519 pk);

TW_EXPORT_METHOD
bool TWPublicKeyEd25519Verify(struct TWPublicKeyEd25519 pk, TWData *_Nonnull signature, TWData *_Nonnull message);

TW_EXPORT_PROPERTY
TWString *_Nonnull TWPublicKeyEd25519Description(struct TWPublicKeyEd25519 publicKey);

TW_EXTERN_C_END
