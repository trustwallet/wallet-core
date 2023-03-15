// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "PublicKey.h"

#include <array>
#include <vector>

namespace TW::Ontology {

class Address {

  private:
    Data toScriptHash(const Data& data);

  public:
    static const size_t size = 20;
    static const uint8_t version = 0x17;

    std::array<uint8_t, size> _data;

    /// Initializes an address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Initializes an address with a string representation.
    explicit Address(const std::string& b58Address);

    /// Initializes an address with a collection of bytes.
    explicit Address(const std::vector<uint8_t>& bytes);

    /// Initializes an address with a collection of public key.
    Address(uint8_t m, const std::vector<Data>& publicKeys);

    /// Determines whether a string makes a valid  address.
    static bool isValid(const std::string& string) noexcept;

    /// Returns a string representation of the address.
    std::string string() const;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs._data == rhs._data;
}

} // namespace TW::Ontology
