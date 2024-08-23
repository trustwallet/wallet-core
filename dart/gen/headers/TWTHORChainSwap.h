// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// THORChain swap functions
TW_EXPORT_STRUCT
struct TWTHORChainSwap;

/// Builds a THORChainSwap transaction input.
///
/// \param input The serialized data of SwapInput.
/// \return The serialized data of SwapOutput.
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWTHORChainSwapBuildSwap(TWData *_Nonnull input);

TW_EXTERN_C_END
