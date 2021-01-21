// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Avalanche {

class Address {
  public:

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Initializes a Avalanche address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes a Avalanche address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;

    std::array<byte, PublicKey::secp256k1Size> bytes;

    std::string hrp;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    // prefer string over byte comparison because string encodes chain and net
    return lhs.string() == rhs.string();
}

inline bool operator!=(const Address& lhs, const Address& rhs) {
    return lhs.string() != rhs.string();
}

inline bool operator<(const Address& lhs, const Address& rhs) {
    // prefer lexicographical comparison of bytes for encoding
    return std::lexicographical_compare(lhs.bytes.begin(), lhs.bytes.end(), rhs.bytes.begin(), rhs.bytes.end());
}

} // namespace TW::Avalanche

/// Wrapper for C interface.
struct TWAvalancheAddress {
    TW::Avalanche::Address impl;
};
