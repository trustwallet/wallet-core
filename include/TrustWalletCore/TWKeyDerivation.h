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

TW_EXPORT_STRUCT
struct TWKeyDerivation {
    uint8_t unused; // C doesn't allow zero-sized struct
};

/// Scrypt key derivation function.
///
/// \param password password.
/// \param salt salt.
/// \param n CPU AND RAM cost (first modifier)
/// \param r RAM Cost
/// \param p CPU cost (parallelisation)
/// \param keyLength desired key length.
TW_EXPORT_STATIC_METHOD
TWData *_Nullable TWKeyDerivationScrypt(TWString *_Nonnull password, TWData *_Nonnull salt, uint64_t n, uint32_t r, uint32_t p, size_t keyLength);

/// PBKDF2 key derivation function, 256-bit variation.
///
/// \param password password.
/// \param salt salt.
/// \param iterations CPU cost.
/// \param keyLength desired key length.
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWKeyDerivationPBKDF2_256(TWString *_Nonnull password, TWData *_Nonnull salt, uint32_t iterations, size_t keyLength);

/// PBKDF2 key derivation function, 512-bit variation.
///
/// \param password password.
/// \param salt salt.
/// \param iterations CPU cost.
/// \param keyLength desired key length.
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWKeyDerivationPBKDF2_512(TWString *_Nonnull password, TWData *_Nonnull salt, uint32_t iterations, size_t keyLength);

TW_EXTERN_C_END
