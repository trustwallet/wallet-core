// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

static const char *_Nonnull IOTEX_STAKING_CONTRACT = "io1xpq62aw85uqzrccg9y5hnryv8ld2nkpycc3gza";

TW_EXPORT_CLASS
struct TWIoTeXStaking;

/// Function to generate Stake message
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWIoTeXStakingStake(TWData *_Nonnull canName, uint64_t stakeDuration, bool nonDecay, TWData *_Nullable data);

/// Function to generate Unstake message
TW_EXPORT_STATIC_METHOD
TWData*_Nonnull TWIoTeXStakingUnstake(uint64_t pyggIndex, TWData *_Nullable data);

/// Function to generate Withdraw message
TW_EXPORT_STATIC_METHOD
TWData*_Nonnull TWIoTeXStakingWithdraw(uint64_t pyggIndex, TWData *_Nullable data);

/// Function to generate AddStake message
TW_EXPORT_STATIC_METHOD
TWData*_Nonnull TWIoTeXStakingAddStake(uint64_t pyggIndex, TWData *_Nullable data);

/// Function to generate MoveStake message
TW_EXPORT_STATIC_METHOD
TWData*_Nonnull TWIoTeXStakingMoveStake(uint64_t pyggIndex, TWData *_Nonnull canName, TWData *_Nullable data);

TW_EXTERN_C_END
