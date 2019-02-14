// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

static const size_t TWPublicKeyCompressedSize = 33;
static const size_t TWPublicKeyUncompressedSize = 65;

TW_EXPORT_STRUCT
struct TWPublicKey {
    uint8_t bytes[TWPublicKeyUncompressedSize];
};

TW_EXPORT_STATIC_METHOD
bool TWPublicKeyInitWithData(struct TWPublicKey *_Nonnull pk, TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
bool TWPublicKeyIsValid(TWData *_Nonnull data);

TW_EXPORT_PROPERTY
bool TWPublicKeyIsCompressed(struct TWPublicKey pk);

TW_EXPORT_PROPERTY
struct TWPublicKey TWPublicKeyCompressed(struct TWPublicKey from);

TW_EXPORT_PROPERTY
TWData *_Nonnull TWPublicKeyData(struct TWPublicKey pk);

TW_EXPORT_METHOD
bool TWPublicKeyVerify(struct TWPublicKey pk, TWData *_Nonnull signature, TWData *_Nonnull message);

TW_EXPORT_PROPERTY
TWString *_Nonnull TWPublicKeyDescription(struct TWPublicKey publicKey);

TW_EXPORT_STATIC_METHOD
struct TWPublicKey TWPublicKeyRecover(TWData *_Nonnull signature, TWData *_Nonnull message);

TW_EXTERN_C_END
