// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKeySecp256k1.h"

#include <stdint.h>
#include <string>

using namespace TW::secp256k1;

namespace TW {
namespace Tendermint {

class Address {
public:
    /// Human-readable part.
    ///
    /// \see https://github.com/satoshilabs/slips/blob/master/slip-0173.md
    std::string hrp;

    /// Public key hash.
    std::vector<uint8_t> keyHash;

    /// Determines whether a string makes a valid Tendermint address.
    static bool isValid(const std::string& string);

    /// Initializes an address with a key hash.
    Address(const std::string& hrp, const std::vector<uint8_t>& keyHash) : hrp(hrp), keyHash(keyHash) {}

    /// Initializes an address with a key hash.
    Address(const std::string& hrp, std::vector<uint8_t>&& keyHash) : hrp(hrp), keyHash(keyHash) {}

    /// Initializes an address with a public key.
    Address(const std::string& hrp, const PublicKey& publicKey);

    /// Decodes an address.
    ///
    /// \returns a pair with the address and a success flag.
    static std::pair<Address, bool> decode(const std::string& addr);

    /// Encodes the address.
    ///
    /// \returns encoded address string, or empty string on failure.
    std::string encode() const;

    bool operator==(const Address& rhs) const {
        return hrp == rhs.hrp && keyHash == rhs.keyHash;
    }

private:
    Address() = default;
};

}} // namespace

/// Wrapper for C interface.
struct TWTendermintAddress {
    TW::Tendermint::Address impl;
};
