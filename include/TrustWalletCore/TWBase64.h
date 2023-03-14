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

/// Base64 encode / decode functions
TW_EXPORT_STRUCT
struct TWBase64;

/// Decode a Base64 input with the default alphabet (RFC4648 with '+', '/')
///
/// \param string Encoded input to be decoded
/// \return The decoded data, empty if decoding failed.
TW_EXPORT_STATIC_METHOD
TWData* _Nullable TWBase64Decode(TWString* _Nonnull string);

/// Decode a Base64 input with the alphabet safe for URL-s and filenames (RFC4648 with '-', '_')
///
/// \param string Encoded base64 input to be decoded
/// \return The decoded data, empty if decoding failed.
TW_EXPORT_STATIC_METHOD
TWData* _Nullable TWBase64DecodeUrl(TWString* _Nonnull string);

/// Encode an input to Base64 with the default alphabet (RFC4648 with '+', '/')
///
/// \param data Data to be encoded (raw bytes)
/// \return The encoded data
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWBase64Encode(TWData *_Nonnull data);

/// Encode an input to Base64 with the alphabet safe for URL-s and filenames (RFC4648 with '-', '_')
///
/// \param data Data to be encoded (raw bytes)
/// \return The encoded data
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWBase64EncodeUrl(TWData *_Nonnull data);

TW_EXTERN_C_END
