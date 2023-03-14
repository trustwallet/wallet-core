// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "../PublicKey.h"
#include "../Bech32Address.h"

#include <string>

namespace TW::Oasis {

/// Oasis address is a Bech32Address, with "oasis" prefix.
class Address: public Bech32Address {
  public:
    /// Number of bytes in an address.
    static constexpr size_t size = 32;
    static const std::string hrp; //HRP_OASIS

    Address() : Bech32Address(hrp) {}

    /// Initializes an address with a key hash.
    explicit Address(const Data& keyHash);

    /// Initializes a Oasis address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Initializes a Oasis address with a string representation.
    explicit Address(const std::string& addr);

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    static bool decode(const std::string& addr, Address& obj_out) {
        return Bech32Address::decode(addr, obj_out, hrp);
    }
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.string() == rhs.string();
}

} // namespace TW::Oasis
