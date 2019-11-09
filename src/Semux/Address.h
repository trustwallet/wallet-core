// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Hash.h"
#include "../PublicKey.h"
#include <TrustWalletCore/TWX509.h>

#include <string>

namespace TW::Semux {

class Address {
  public:
    /// Number of bytes in an address.
    static const size_t size = 20;

    /// Address data followed by the public key
    std::array<uint8_t, size> bytes;

    /// Determines whether a collection of bytes makes a valid  address.
    static bool isValid(const Data &data) { return data.size() == size; }

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string &string);

    /// Initializes an address from a string representation.
    Address(const std::string &string);

    /// Initializes an address from a public key.
    Address(const PublicKey &publicKey);

    /// Returns a string representation of the address. 20-bytes hex-encoded
    std::string string() const;
};

inline bool operator==(const Address &lhs, const Address &rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::Semux

/// Wrapper for C interface.
struct TWSemuxAddress {
    TW::Semux::Address impl;
};
