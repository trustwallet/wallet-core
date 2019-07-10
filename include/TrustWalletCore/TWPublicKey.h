// Copyright © 2017-2019 Trust Wallet.
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

TW_EXPORT_CLASS
struct TWPublicKey;

TW_EXPORT_STATIC_METHOD
struct TWPublicKey *_Nullable TWPublicKeyCreateWithData(TWData *_Nonnull data, enum TWPublicKeyType type);

TW_EXPORT_METHOD
void TWPublicKeyDelete(struct TWPublicKey *_Nonnull pk);

TW_EXPORT_STATIC_METHOD
bool TWPublicKeyIsValid(TWData *_Nonnull data, enum TWPublicKeyType type);

TW_EXPORT_PROPERTY
bool TWPublicKeyIsCompressed(struct TWPublicKey *_Nonnull pk);

TW_EXPORT_PROPERTY
struct TWPublicKey *_Nonnull TWPublicKeyCompressed(struct TWPublicKey *_Nonnull from);

TW_EXPORT_PROPERTY
struct TWPublicKey *_Nonnull TWPublicKeyUncompressed(struct TWPublicKey *_Nonnull from);

TW_EXPORT_PROPERTY
TWData *_Nonnull TWPublicKeyData(struct TWPublicKey *_Nonnull pk);

TW_EXPORT_METHOD
bool TWPublicKeyVerify(struct TWPublicKey *_Nonnull pk, TWData *_Nonnull signature, TWData *_Nonnull message);

TW_EXPORT_METHOD
bool TWPublicKeyVerifySchnorr(struct TWPublicKey *_Nonnull pk, TWData *_Nonnull signature, TWData *_Nonnull message);

TW_EXPORT_PROPERTY
enum TWPublicKeyType TWPublicKeyKeyType(struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_PROPERTY
TWString *_Nonnull TWPublicKeyDescription(struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_STATIC_METHOD
struct TWPublicKey *_Nullable TWPublicKeyRecover(TWData *_Nonnull signature, TWData *_Nonnull message);

TW_EXTERN_C_END
