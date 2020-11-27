// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"

#include <vector>
#include <string>

namespace TW::Solana {

class StakeProgram {
public:
    static Address addressFromValidatorSeed(const TW::Solana::Address& fromAddress,
                                            const TW::Solana::Address& validatorAddress,
                                            const TW::Solana::Address& programId);
};


class TokenProgram {
public:
    /// Derive default token address for main address and token
    static std::string defaultTokenAddress(const std::string& mainAddress, const std::string& tokenMintAddress);

    /// Create a new valid address, if neeed, trying several
    static std::string findProgramAddress(const std::vector<TW::Data>& seeds, const Address& programId);

    /// Create a new address for program, with given seeds
    static Address createProgramAddress(const std::vector<TW::Data>& seeds, const Address& programId);
};

} // namespace TW::Solana
