// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

/// Hash functions
TW_EXPORT_STRUCT
struct TWHash {
    uint8_t unused; // C doesn't allow zero-sized struct
};

static const size_t TWHashSHA1Length = 20;
static const size_t TWHashSHA256Length = 32;
static const size_t TWHashSHA512Length = 64;
static const size_t TWHashRipemdLength = 20;

/// Computes the SHA1 of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed SHA1 block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA1(TWData *_Nonnull data);

/// Computes the SHA256 of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed SHA256 block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA256(TWData *_Nonnull data);

/// Computes the SHA512 of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed SHA512 block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA512(TWData *_Nonnull data);

/// Computes the SHA512_256 of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed SHA512_256 block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA512_256(TWData *_Nonnull data);

/// Computes the Keccak256 of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed Keccak256 block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashKeccak256(TWData *_Nonnull data);

/// Computes the Keccak512 of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed Keccak512 block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashKeccak512(TWData *_Nonnull data);

/// Computes the SHA3_256 of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed SHA3_256 block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA3_256(TWData *_Nonnull data);

/// Computes the SHA3_512 of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed SHA3_512 block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA3_512(TWData *_Nonnull data);

/// Computes the RIPEMD of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed RIPEMD block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashRIPEMD(TWData *_Nonnull data);

/// Computes the Blake256 of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed Blake256 block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashBlake256(TWData *_Nonnull data);

/// Computes the Blake2b of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed Blake2b block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashBlake2b(TWData *_Nonnull data, size_t size);

/// Computes the Groestl512 of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed Groestl512 block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashBlake2bPersonal(TWData *_Nonnull data, TWData * _Nonnull personal, size_t outlen);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashGroestl512(TWData *_Nonnull data);

/// Computes the SHA256D of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed SHA256D block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA256SHA256(TWData *_Nonnull data);

/// Computes the SHA256RIPEMD of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed SHA256RIPEMD block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA256RIPEMD(TWData *_Nonnull data);

/// Computes the SHA3_256RIPEMD of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed SHA3_256RIPEMD block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashSHA3_256RIPEMD(TWData *_Nonnull data);

/// Computes the Blake256D of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed Blake256D block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashBlake256Blake256(TWData *_Nonnull data);

/// Computes the Blake256RIPEMD of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed Blake256RIPEMD block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashBlake256RIPEMD(TWData *_Nonnull data);

/// Computes the Groestl512D of a block of data.
///
/// \param data Non-null block of data
/// \return Non-null computed Groestl512D block of data
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWHashGroestl512Groestl512(TWData *_Nonnull data);

TW_EXTERN_C_END
