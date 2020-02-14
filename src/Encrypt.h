// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

namespace TW::Encrypt {

enum PaddingMode {
    PadWithZeros = 0, // padding value is zero
    PadWithPaddingSize // padding value is the number of padding bytes; for even size add an extra block (PKCS#7)
};

/// Determind needed padding size (used internally)
size_t paddingSize(size_t origSize, size_t blockSize, PaddingMode paddingMode);

/// Encrypts a block of data using AES in Cipher Block Chaining (CBC) mode.
///
/// \param key encryption key, must be 16/24/32/128/192/256 bytes long.
/// \param data data to encrypt.
/// \param iv initialization vector.
/// \param paddingMode If PadWithZeroes (default), data is padded with 0's to even block size.
///                    If PadWithPaddingSize, pad value is padding size, and even size input is padded with an extra block.
Data AESCBCEncrypt(const Data& key, const Data& data, Data& iv, PaddingMode paddingMode = PadWithZeros);

/// Decrypts a block of data using AES in Cipher Block Chaining (CBC) mode.
///
/// \param key decryption key, must be 16, 24, or 32 bytes long.
/// \param data data to decrypt.
/// \param iv initialization vector.
/// \param paddingMode If PadWithZeroes (default), padding is not removed.
///                    If PadWithPaddingSize, padding is removed.
Data AESCBCDecrypt(const Data& key, const Data& data, Data& iv, PaddingMode paddingMode = PadWithZeros);

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
