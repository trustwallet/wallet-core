// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

/// Password-Based Key Derivation Function 2
TW_EXPORT_STRUCT
struct TWPBKDF2;

/// Derives a key from a password and a salt using PBKDF2 + Sha256.
///
/// \param password is the master password from which a derived key is generated
/// \param salt is a sequence of bits, known as a cryptographic salt
/// \param iterations is the number of iterations desired
/// \param dkLen is the desired bit-length of the derived key
/// \return the derived key data.
TW_EXPORT_STATIC_METHOD
TWData *_Nullable TWPBKDF2HmacSha256(TWData *_Nonnull password, TWData *_Nonnull salt, uint32_t iterations, uint32_t dkLen);

/// Derives a key from a password and a salt using PBKDF2 + Sha512.
///
/// \param password is the master password from which a derived key is generated
/// \param salt is a sequence of bits, known as a cryptographic salt
/// \param iterations is the number of iterations desired
/// \param dkLen is the desired bit-length of the derived key
/// \return the derived key data.
TW_EXPORT_STATIC_METHOD
TWData *_Nullable TWPBKDF2HmacSha512(TWData *_Nonnull password, TWData *_Nonnull salt, uint32_t iterations, uint32_t dkLen);

TW_EXTERN_C_END
