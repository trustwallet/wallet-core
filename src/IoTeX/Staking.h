// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "proto/IoTeX.pb.h"
namespace TW::IoTeX {

/// Function to generate Create message
Data stakingCreate(const Data& candidate, const Data& amount, uint32_t duration, bool autoStake,
                   const Data& payload);

/// Function to generate AddDeposit message
Data stakingAddDeposit(uint64_t index, const Data& amount, const Data& payload);

/// Function to generate Unstake message
Data stakingUnstake(uint64_t index, const Data& payload);

/// Function to generate Withdraw message
Data stakingWithdraw(uint64_t index, const Data& payload);

/// Function to generate Restake message
Data stakingRestake(uint64_t index, uint32_t duration, bool autoStake, const Data& payload);

/// Function to generate ChangeCandidate message
Data stakingChangeCandidate(uint64_t index, const Data& candidate, const Data& payload);

/// Function to generate Transfer message
Data stakingTransfer(uint64_t index, const Data& voterAddress, const Data& payload);

/// Function to generate candidate register message
Data candidateRegister(const Data& name, const Data& operatorAddress, const Data& rewardAddress,
                       const Data& amount, uint32_t duration, bool autoStake,
                       const Data& ownerAddress, const Data& payload);

/// Function to generate candidate update message
Data candidateUpdate(const Data& name, const Data& operatorAddress, const Data& rewardAddress);

} // namespace TW::IoTeX
