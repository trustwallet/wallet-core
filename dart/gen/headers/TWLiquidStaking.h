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
struct TWLiquidStaking;

/// Builds a LiquidStaking transaction input.
///
/// \param input The serialized data of LiquidStakingInput.
/// \return The serialized data of LiquidStakingOutput.
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWLiquidStakingBuildRequest(TWData *_Nonnull input);

TW_EXTERN_C_END
