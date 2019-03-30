// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include <cstdint>
#include <string>

namespace TW::Tendermint {

class Address {
  public:
    /// Human-readable part.
    ///
    /// \see https://github.com/satoshilabs/slips/blob/master/slip-0173.md
    std::string hrp;

    /// Public key hash.
    Data keyHash;

    /// Determines whether a string makes a valid Tendermint address.
    static bool isValid(const std::string& string);

    /// Determines whether a string makes a valid Tendermint address, and the
    /// HRP matches.
    static bool isValid(const std::string& string, const std::string& hrp);

    /// Initializes an address with a key hash.
    Address(std::string hrp, Data keyHash) : hrp(std::move(hrp)), keyHash(std::move(keyHash)) {}

    /// Initializes an address with a public key.
    Address(std::string hrp, const PublicKey& publicKey);

    /// Decodes an address.
    ///
    /// \returns a pair with the address and a success flag.
    static std::pair<Address, bool> decode(const std::string& addr);

    /// Encodes the address.
    ///
    /// \returns encoded address string, or empty string on failure.
    std::string string() const;

    bool operator==(const Address& rhs) const { return hrp == rhs.hrp && keyHash == rhs.keyHash; }

  private:
    Address() = default;
};

} // namespace TW::Tendermint

/// Wrapper for C interface.
struct TWTendermintAddress {
    TW::Tendermint::Address impl;
};
