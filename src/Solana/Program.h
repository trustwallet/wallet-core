// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Address.h"
#include "Transaction.h"

#include <vector>

namespace TW::Solana {

class StakeProgram {
public:
    static Address addressFromValidatorSeed(const Address& fromAddress,
                                            const Address& validatorAddress,
                                            const Address& programId);

    static Address addressFromRecentBlockhash(const Address& fromAddress, const Data& recentBlockhash, const Address& programId);
};


class TokenProgram {
public:
    /// Derive default token address for main address and token
    static Address defaultTokenAddress(const Address& mainAddress, const Address& tokenMintAddress);

    /// Create a new valid address, if neeed, trying several
    static Address findProgramAddress(const std::vector<TW::Data>& seeds, const Address& programId);

    /// Create a new address for program, with given seeds
    static Address createProgramAddress(const std::vector<TW::Data>& seeds, const Address& programId);
};

} // namespace TW::Solana
