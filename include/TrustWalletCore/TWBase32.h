// Copyright © 2017-2022 Trust Wallet.
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
struct TWBase32;

/**
 * @param string Encoded base32 input to be decoded
 * @param alphabet Decode with the given alphabet, if nullptr ALPHABET_RFC4648 is used by default
 * @return The decoded data
 * @note ALPHABET_RFC4648 doesn't support padding in the default alphabet
 * @details Decode a Base32 input with the given alphabet
 */
TW_EXPORT_STATIC_METHOD
TWData* _Nullable TWBase32DecodeWithAlphabet(TWString* _Nonnull string, TWString* _Nullable alphabet);

/**
 * @param string Encoded input to be decoded
 * @return The decoded data
 * @note Call TWBase32DecodeWithAlphabet with nullable alphabet.
 * @details Decode a Base32 input with the default alphabet (ALPHABET_RFC4648)
 */
TW_EXPORT_STATIC_METHOD
TWData* _Nullable TWBase32Decode(TWString* _Nonnull string);

TW_EXTERN_C_END
