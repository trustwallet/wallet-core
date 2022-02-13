// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"
#include "TWData.h"

// Wrapper class for Ethereum EIP 1559 Fee suggestion

TW_EXTERN_C_BEGIN

TW_EXPORT_STRUCT
struct TWEthereumFee;

/// Suggest baseFee and maxPriorityFee based on eth_feeHistory RPC call response
TW_EXPORT_STATIC_METHOD
TWString* _Nullable TWEthereumFeeSuggest(TWString* _Nonnull feeHistory);

TW_EXTERN_C_END
