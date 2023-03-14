// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

/// Represents Ethereum ABI value
TW_EXPORT_STRUCT
struct TWEthereumAbiValue;

/// Encode a bool according to Ethereum ABI, into 32 bytes.  Values are padded by 0 on the left, unless specified otherwise
///
/// \param value a boolean value
/// \return Encoded value stored in a block of data
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeBool(bool value);

/// Encode a int32 according to Ethereum ABI, into 32 bytes. Values are padded by 0 on the left, unless specified otherwise
///
/// \param value a int32 value
/// \return Encoded value stored in a block of data
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeInt32(int32_t value);

/// Encode a uint32 according to Ethereum ABI, into 32 bytes.  Values are padded by 0 on the left, unless specified otherwise
///
/// \param value a uint32 value
/// \return Encoded value stored in a block of data
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeUInt32(uint32_t value);

/// Encode a int256 according to Ethereum ABI, into 32 bytes.  Values are padded by 0 on the left, unless specified otherwise
///
/// \param value a int256 value stored in a block of data
/// \return Encoded value stored in a block of data
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeInt256(TWData* _Nonnull value);

/// Encode an int256 according to Ethereum ABI, into 32 bytes.  Values are padded by 0 on the left, unless specified otherwise
///
/// \param value a int256 value stored in a block of data
/// \return Encoded value stored in a block of data
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeUInt256(TWData* _Nonnull value);

/// Encode an address according to Ethereum ABI, 20 bytes of the address.
///
/// \param value an address value stored in a block of data
/// \return Encoded value stored in a block of data
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeAddress(TWData* _Nonnull value);

/// Encode a string according to Ethereum ABI by encoding its hash.
///
/// \param value a string value
/// \return Encoded value stored in a block of data
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeString(TWString* _Nonnull value);

/// Encode a number of bytes, up to 32 bytes, padded on the right.  Longer arrays are truncated.
///
/// \param value bunch of bytes
/// \return Encoded value stored in a block of data
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeBytes(TWData* _Nonnull value);

/// Encode a dynamic number of bytes by encoding its hash
///
/// \param value bunch of bytes
/// \return Encoded value stored in a block of data
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncodeBytesDyn(TWData* _Nonnull value);

/// Decodes input data (bytes longer than 32 will be truncated) as uint256
///
/// \param input Data to be decoded
/// \return Non-null decoded string value
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWEthereumAbiValueDecodeUInt256(TWData* _Nonnull input);

/// Decode an arbitrary type, return value as string
///
/// \param input Data to be decoded
/// \param type the underlying type that need to be decoded
/// \return Non-null decoded string value
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWEthereumAbiValueDecodeValue(TWData* _Nonnull input, TWString* _Nonnull type);

/// Decode an array of given simple types.  Return a '\n'-separated string of elements
///
/// \param input Data to be decoded
/// \param type the underlying type that need to be decoded
/// \return Non-null decoded string value
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWEthereumAbiValueDecodeArray(TWData* _Nonnull input, TWString* _Nonnull type);

TW_EXTERN_C_END
