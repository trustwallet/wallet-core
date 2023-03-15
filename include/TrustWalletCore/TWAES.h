// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWAESPaddingMode.h"

TW_EXTERN_C_BEGIN

/// AES encryption/decryption methods.
TW_EXPORT_STRUCT
struct TWAES {
    uint8_t unused; // C doesn't allow zero-sized struct
};

/// Encrypts a block of Data using AES in Cipher Block Chaining (CBC) mode.
///
/// \param key encryption key Data, must be 16, 24, or 32 bytes long.
/// \param data Data to encrypt.
/// \param iv initialization vector.
/// \param mode padding mode.
/// \return encrypted Data.
TW_EXPORT_STATIC_METHOD
TWData *_Nullable TWAESEncryptCBC(TWData *_Nonnull key, TWData *_Nonnull data, TWData *_Nonnull iv, enum TWAESPaddingMode mode);

/// Decrypts a block of data using AES in Cipher Block Chaining (CBC) mode.
///
/// \param key decryption key Data, must be 16, 24, or 32 bytes long.
/// \param data Data to decrypt.
/// \param iv initialization vector Data.
/// \param mode padding mode.
/// \return decrypted Data.
TW_EXPORT_STATIC_METHOD
TWData *_Nullable TWAESDecryptCBC(TWData *_Nonnull key, TWData *_Nonnull data, TWData *_Nonnull iv, enum TWAESPaddingMode mode);

/// Encrypts a block of data using AES in Counter (CTR) mode.
///
/// \param key encryption key Data, must be 16, 24, or 32 bytes long.
/// \param data Data to encrypt.
/// \param iv initialization vector Data.
/// \return encrypted Data.
TW_EXPORT_STATIC_METHOD
TWData *_Nullable TWAESEncryptCTR(TWData *_Nonnull key, TWData *_Nonnull data, TWData *_Nonnull iv);

/// Decrypts a block of data using AES in Counter (CTR) mode.
///
/// \param key decryption key Data, must be 16, 24, or 32 bytes long.
/// \param data Data to decrypt.
/// \param iv initialization vector Data.
/// \return decrypted Data.
TW_EXPORT_STATIC_METHOD
TWData *_Nullable TWAESDecryptCTR(TWData *_Nonnull key, TWData *_Nonnull data, TWData *_Nonnull iv);

TW_EXTERN_C_END
