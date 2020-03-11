// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

namespace TW::IoTeX {

/// Function to generate Stake message
void stakingStake(const Data& candidate, uint64_t stakeDuration, bool nonDecay, const Data& dataIn, Data& dataOut);

/// Function to generate Unstake message
void stakingUnstake(uint64_t pyggIndex, const Data& dataIn, Data& dataOut);

/// Function to generate Withdraw message
void stakingWithdraw(uint64_t pyggIndex, const Data& dataIn, Data& dataOut);

/// Function to generate AddStake message
void stakingAddStake(uint64_t pyggIndex, const Data& dataIn, Data& dataOut);

/// Function to generate MoveStake message
void stakingMoveStake(uint64_t pyggIndex, const Data& candidate, const Data& dataIn, Data& dataOut);

} // namespace TW::IoTeX
