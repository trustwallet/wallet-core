// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Base58Address.h"
#include "../PublicKey.h"

#include <array>
#include <cstdint>
#include <vector>

namespace TW::Solana {

const std::string STAKE_ADDRESS = "Stake11111111111111111111111111111111111111";

class Address : public Base58Address<32> {
  public:
    /// Determines whether a collection of bytes makes a valid Solana address.
    static bool isValid(const Data& data) { return data.size() == size; }

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Initializes an address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes an address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;

    /// Returns a vector of the address.
    Data vector() const;
};

} // namespace TW::Solana

TW::Solana::Address addressFromValidatorSeed(TW::Solana::Address& fromAddress,
                                             TW::Solana::Address& validatorAddress,
                                             TW::Solana::Address& programId);
