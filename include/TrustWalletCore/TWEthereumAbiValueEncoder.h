// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWBase.h>
#include <TrustWalletCore/TWString.h>
#include <TrustWalletCore/TWData.h>

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWEthereumAbiValueEncoder;

/// Returned data must be deleted (hint: use WRAPD() macro).
/// Encode a type according to EIP712, into 32 bytes.  Values are padded by 0 on the left, unless specified otherwise.
/// See: https://github.com/ethereum/EIPs/blob/master/EIPS/eip-712.md

TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncoderEncodeBool(bool value);

TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncoderEncodeInt32(int32_t value);

TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncoderEncodeUInt32(uint32_t value);

/// Encode an int256.  Input value is represented as a 32-byte value
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncoderEncodeInt256(TWData* _Nonnull value);

/// Encode an uint256.  Input value is represented as a 32-byte binary value
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncoderEncodeUInt256(TWData* _Nonnull value);

/// Encode the 20 bytes of an address
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncoderEncodeAddress(TWData* _Nonnull value);

/// Encode a string by encoding its hash
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncoderEncodeString(TWString* _Nonnull value);

/// Encode a number of bytes, up to 32 bytes, padded on the right.  Longer arrays are truncated.
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncoderEncodeBytes(TWData* _Nonnull value);

/// Encode a dynamic number of bytes by encoding its hash
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiValueEncoderEncodeBytesDyn(TWData* _Nonnull value);

TW_EXTERN_C_END
