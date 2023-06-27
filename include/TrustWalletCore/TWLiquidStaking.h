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
