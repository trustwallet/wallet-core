// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../Base58Address.h"
#include "../PublicKey.h"

#include <vector>
#include <string>

namespace TW::Solana {

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

    /// Initializes an address with public key data.
    explicit Address(const Data& publicKeyData);

    /// Returns a string representation of the address.
    std::string string() const;

    /// Returns a vector of the address.
    Data vector() const;

    /// Derive default token address for token
    Address defaultTokenAddress(const Address& tokenMintAddress);
};

} // namespace TW::Solana

/// Wrapper for C interface
struct TWSolanaAddress {
    TW::Solana::Address impl;
};
