// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"
#include "TWData.h"

// Wrapper class for Ethereum ABI encoding & decoding.

TW_EXTERN_C_BEGIN

struct TWEthereumAbiFunction;

TW_EXPORT_CLASS
struct TWEthereumAbi;

/// Encode function to Eth ABI binary
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumAbiEncode(struct TWEthereumAbiFunction* _Nonnull fn);

/// Decode function output from Eth ABI binary, fill output parameters
TW_EXPORT_STATIC_METHOD
bool TWEthereumAbiDecodeOutput(struct TWEthereumAbiFunction* _Nonnull fn, TWData* _Nonnull encoded);

/// Decode function call data to human readable json format, according to input abi json
TW_EXPORT_STATIC_METHOD
TWString* _Nullable TWEthereumAbiDecodeCall(TWData* _Nonnull data, TWString* _Nonnull abi);

TW_EXTERN_C_END
