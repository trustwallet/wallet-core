// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWEthereumAbiValueEncode;

/// Encode a type according to EIP712, into 32 bytes.  Values are padded by 0 on the left, if not specified otherwise.
/// To prevent potential issue with forgotten Delete methods, the encode methods do not return a Data struct,
/// but fill in the provided data byte vector.  Its size must be 32 bytes.
/// See: https://github.com/ethereum/EIPs/blob/master/EIPS/eip-712.md

TW_EXPORT_STATIC_METHOD
void TWEthereumAbiValueEncodeBool(bool value, TWData* _Nonnull data_inout);

TW_EXPORT_STATIC_METHOD
void TWEthereumAbiValueEncodeInt32(int32_t value, TWData* _Nonnull data_inout);

TW_EXPORT_STATIC_METHOD
void TWEthereumAbiValueEncodeUInt32(uint32_t value, TWData* _Nonnull data_inout);

/// Encode an int256.  Input value is represented as a 32-byte value
TW_EXPORT_STATIC_METHOD
void TWEthereumAbiValueEncodeInt256(TWData* _Nonnull value, TWData* _Nonnull data_inout);

/// Encode an uint256.  Input value is represented as a 32-byte binary value
TW_EXPORT_STATIC_METHOD
void TWEthereumAbiValueEncodeUInt256(TWData* _Nonnull value, TWData* _Nonnull data_inout);

/// Encode the 20 bytes of an address
TW_EXPORT_STATIC_METHOD
void TWEthereumAbiValueEncodeAddress(TWData* _Nonnull value, TWData* _Nonnull data_inout);

/// Encode a string by encoding its hash
TW_EXPORT_STATIC_METHOD
void TWEthereumAbiValueEncodeString(TWString* _Nonnull value, TWData* _Nonnull data_inout);

/// Encode a number of bytes, up to 32 bytes, padded on the right.  Longer arrays are truncated.
TW_EXPORT_STATIC_METHOD
void TWEthereumAbiValueEncodeBytes(TWData* _Nonnull value, TWData* _Nonnull data_inout);

/// Encode a dynamic number of bytes by encoding its hash
TW_EXPORT_STATIC_METHOD
void TWEthereumAbiValueEncodeBytesDyn(TWData* _Nonnull value, TWData* _Nonnull data_inout);

TW_EXTERN_C_END
