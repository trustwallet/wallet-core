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

static const size_t TWPublicKeySecp256k1CompressedSize = 33;
static const size_t TWPublicKeySecp256k1UncompressedSize = 65;

TW_EXPORT_STRUCT
struct TWPublicKeySecp256k1 {
    uint8_t bytes[TWPublicKeySecp256k1UncompressedSize];
};

TW_EXPORT_STATIC_METHOD
bool TWPublicKeySecp256k1InitWithData(struct TWPublicKeySecp256k1 *_Nonnull pk, TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
bool TWPublicKeySecp256k1IsValid(TWData *_Nonnull data);

TW_EXPORT_PROPERTY
bool TWPublicKeySecp256k1IsCompressed(struct TWPublicKeySecp256k1 pk);

TW_EXPORT_PROPERTY
struct TWPublicKeySecp256k1 TWPublicKeySecp256k1Compressed(struct TWPublicKeySecp256k1 from);

TW_EXPORT_PROPERTY
TWData *_Nonnull TWPublicKeySecp256k1Data(struct TWPublicKeySecp256k1 pk);

TW_EXPORT_METHOD
bool TWPublicKeySecp256k1Verify(struct TWPublicKeySecp256k1 pk, TWData *_Nonnull signature, TWData *_Nonnull message);

TW_EXPORT_PROPERTY
TWString *_Nonnull TWPublicKeySecp256k1Description(struct TWPublicKeySecp256k1 publicKey);

TW_EXPORT_STATIC_METHOD
struct TWPublicKeySecp256k1 TWPublicKeySecp256k1Recover(TWData *_Nonnull signature, TWData *_Nonnull message);

TW_EXTERN_C_END
