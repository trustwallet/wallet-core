// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Bech32 encode / decode functions
TW_EXPORT_STRUCT
struct TWBech32;

/// Encodes data as a Bech32 string.
///
/// \param hrp The human-readable part.
/// \param data The data part.
/// \return the encoded Bech32 string.
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWBech32Encode(TWString* _Nonnull hrp, TWData *_Nonnull data);

/// Decodes a Bech32 string. Returns null if the string is not a valid Bech32 string.
///
/// \param string The Bech32 string to decode.
/// \return the decoded data, null if the string is not a valid Bech32 string. Note that the human-readable part is not returned.
TW_EXPORT_STATIC_METHOD
TWData *_Nullable TWBech32Decode(TWString *_Nonnull string);

/// Encodes data as a Bech32m string.
///
/// \param hrp The human-readable part.
/// \param data The data part.
/// \return the encoded Bech32m string.
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWBech32EncodeM(TWString* _Nonnull hrp, TWData *_Nonnull data);

/// Decodes a Bech32m string. Returns null if the string is not a valid Bech32m string.
///
/// \param string The Bech32m string to decode.
/// \return the decoded data, null if the string is not a valid Bech32m string. Note that the human-readable part is not returned.
TW_EXPORT_STATIC_METHOD
TWData *_Nullable TWBech32DecodeM(TWString *_Nonnull string);

TW_EXTERN_C_END
