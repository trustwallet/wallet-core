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

/// Wrapper class for Ethereum ABI encoding & decoding.
struct TWEthereumAbiFunction;

TW_EXPORT_STRUCT
struct TWEthereumAbi;

/// Encode function to Eth ABI binary
///
/// \param fn Non-null Eth abi function
/// \return Non-null encoded block of data
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiEncode(struct TWEthereumAbiFunction* _Nonnull fn);

/// Decode function output from Eth ABI binary, fill output parameters
///
/// \param[in] fn Non-null Eth abi function
/// \param[out] encoded Non-null block of data
/// \return true if encoded have been filled correctly, false otherwise
TW_EXPORT_STATIC_METHOD
bool TWEthereumAbiDecodeOutput(struct TWEthereumAbiFunction* _Nonnull fn, TWData* _Nonnull encoded);

/// Decode function call data to human readable json format, according to input abi json
///
/// \param data Non-null block of data
/// \param abi Non-null string
/// \return Non-null json string function call data
TW_EXPORT_STATIC_METHOD
TWString* _Nullable TWEthereumAbiDecodeCall(TWData* _Nonnull data, TWString* _Nonnull abi);

/// Compute the hash of a struct, used for signing, according to EIP712 ("v4").
/// Input is a Json object (as string), with following fields:
/// - types: map of used struct types (see makeTypes())
/// - primaryType: the type of the message (string)
/// - domain: EIP712 domain specifier values
/// - message: the message (object).
/// Throws on error.
/// Example input:
///  R"({
///     "types": {
///         "EIP712Domain": [
///             {"name": "name", "type": "string"},
///             {"name": "version", "type": "string"},
///             {"name": "chainId", "type": "uint256"},
///             {"name": "verifyingContract", "type": "address"}
///         ],
///         "Person": [
///             {"name": "name", "type": "string"},
///             {"name": "wallet", "type": "address"}
///         ]
///     },
///     "primaryType": "Person",
///     "domain": {
///         "name": "Ether Person",
///         "version": "1",
///         "chainId": 1,
///         "verifyingContract": "0xCcCCccccCCCCcCCCCCCcCcCccCcCCCcCcccccccC"
///     },
///     "message": {
///         "name": "Cow",
///         "wallet": "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"
///     }
///  })");
/// On error, empty Data is returned.
/// Returned data must be deleted (hint: use WRAPD() macro).
///
/// \param messageJson Non-null json abi input
/// \return Non-null block of data, encoded abi input
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiEncodeTyped(TWString* _Nonnull messageJson);

TW_EXTERN_C_END
