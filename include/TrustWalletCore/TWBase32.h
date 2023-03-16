// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Base32 encode / decode functions
TW_EXPORT_STRUCT
struct TWBase32;

/// Decode a Base32 input with the given alphabet
///
/// \param string Encoded base32 input to be decoded
/// \param alphabet Decode with the given alphabet, if nullptr ALPHABET_RFC4648 is used by default
/// \return The decoded data, can be null.
/// \note ALPHABET_RFC4648 doesn't support padding in the default alphabet
TW_EXPORT_STATIC_METHOD
TWData* _Nullable TWBase32DecodeWithAlphabet(TWString* _Nonnull string, TWString* _Nullable alphabet);

/// Decode a Base32 input with the default alphabet (ALPHABET_RFC4648)
///
/// \param string Encoded input to be decoded
/// \return The decoded data
/// \note Call TWBase32DecodeWithAlphabet with nullptr.
TW_EXPORT_STATIC_METHOD
TWData* _Nullable TWBase32Decode(TWString* _Nonnull string);

/// Encode an input to Base32 with the given alphabet
///
/// \param data Data to be encoded (raw bytes)
/// \param alphabet Encode with the given alphabet, if nullptr ALPHABET_RFC4648 is used by default
/// \return The encoded data
/// \note ALPHABET_RFC4648 doesn't support padding in the default alphabet
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWBase32EncodeWithAlphabet(TWData *_Nonnull data, TWString* _Nullable alphabet);

/// Encode an input to Base32 with the default alphabet (ALPHABET_RFC4648)
///
/// \param data Data to be encoded (raw bytes)
/// \return The encoded data
/// \note Call TWBase32EncodeWithAlphabet with nullptr.
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWBase32Encode(TWData *_Nonnull data);

TW_EXTERN_C_END

