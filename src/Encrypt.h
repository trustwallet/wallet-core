// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

namespace TW::Encrypt {

/// Encrypts a block of data using AES in Cipher Block Chaining (CBC) mode.
///
/// \param key encryption key, must be 16, 24, or 32 bytes long.
/// \param data data to encrypt.
/// \param iv initialization vector.
Data AESCBCEncrypt(const Data& key, const Data& data, Data& iv);

/// Decrypts a block of data using AES in Cipher Block Chaining (CBC) mode.
///
/// \param key decryption key, must be 16, 24, or 32 bytes long.
/// \param data data to decrypt.
/// \param iv initialization vector.
Data AESCBCDecrypt(const Data& key, const Data& data, Data& iv);

/// Encrypts a block of data using AES in Counter (CTR) mode.
///
/// \param key encryption key, must be 16, 24, or 32 bytes long.
/// \param data data to encrypt.
/// \param iv initialization vector.
Data AESCTREncrypt(const Data& key, const Data& data, Data& iv);

/// Decrypts a block of data using AES in Counter (CTR) mode.
///
/// \param key decryption key, must be 16, 24, or 32 bytes long.
/// \param data data to decrypt.
/// \param iv initialization vector.
Data AESCTRDecrypt(const Data& key, const Data& data, Data& iv);

} // namespace TW::Encrypt
