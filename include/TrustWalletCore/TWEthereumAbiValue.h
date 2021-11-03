// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_STRUCT
struct TWEthereumAbiValue;

/// Returned data must be deleted (hint: use WRAPD() macro).
/// Encode a type according to Ethereum ABI, into 32 bytes. Values are padded by 0 on the left, unless specified otherwise.

TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeBool(bool value);

TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeInt32(int32_t value);

TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeUInt32(uint32_t value);

/// Encode an int256.  Input value is represented as a 32-byte value
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeInt256(TWData* _Nonnull value);

/// Encode an uint256.  Input value is represented as a 32-byte binary value
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeUInt256(TWData* _Nonnull value);

/// Encode the 20 bytes of an address
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeAddress(TWData* _Nonnull value);

/// Encode a string by encoding its hash
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeString(TWString* _Nonnull value);

/// Encode a number of bytes, up to 32 bytes, padded on the right.  Longer arrays are truncated.
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeBytes(TWData* _Nonnull value);

/// Encode a dynamic number of bytes by encoding its hash
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeBytesDyn(TWData* _Nonnull value);


/// Decodes input data (bytes longer than 32 will be truncated) as uint256
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWEthereumAbiValueDecodeUInt256(TWData* _Nonnull input);

/// Decode an arbitrary type, return value as string
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWEthereumAbiValueDecodeValue(TWData* _Nonnull input, TWString* _Nonnull type);

/// Decode an array of given simple types.  Return a '\n'-separated string of elements
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWEthereumAbiValueDecodeArray(TWData* _Nonnull input, TWString* _Nonnull type);

TW_EXTERN_C_END
