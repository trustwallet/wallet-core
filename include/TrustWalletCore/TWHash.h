// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_STRUCT
struct TWHash {
    uint8_t unused; // C doesn't allow zero-sized struct
};

static const size_t TWHashSHA1Length = 20;
static const size_t TWHashSHA256Length = 32;
static const size_t TWHashSHA512Length = 64;
static const size_t TWHashRipemdLength = 20;

/// Computes the SHA1 of a block of data.
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA1(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA256(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA512(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA512_256(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashKeccak256(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashKeccak512(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA3_256(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA3_512(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashRIPEMD(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashBlake256(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashBlake2b(TWData *_Nonnull data, size_t size);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashGroestl512(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA256SHA256(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA256RIPEMD(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA3_256RIPEMD(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashBlake256Blake256(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashBlake256RIPEMD(TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashGroestl512Groestl512(TWData *_Nonnull data);

TW_EXTERN_C_END
