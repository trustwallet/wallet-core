// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Cardano helper functions
TW_EXPORT_STRUCT
struct TWCardano;

/// Calculates the minimum ADA amount needed for a UTXO.
///
/// \see reference https://docs.cardano.org/native-tokens/minimum-ada-value-requirement
/// \param tokenBundle serialized data of TW.Cardano.Proto.TokenBundle.
/// \return the minimum ADA amount.
TW_EXPORT_STATIC_METHOD
uint64_t TWCardanoMinAdaAmount(TWData *_Nonnull tokenBundle) TW_VISIBILITY_DEFAULT;

/// Return the staking address associated to (contained in) this address. Must be a Base address.
/// Empty string is returned on error. Result must be freed.
/// \param baseAddress A valid base address, as string.
/// \return the associated staking (reward) address, as string, or empty string on error.
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWCardanoGetStakingAddress(TWString *_Nonnull baseAddress) TW_VISIBILITY_DEFAULT;

TW_EXTERN_C_END
