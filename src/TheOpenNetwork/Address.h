// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PublicKey.h"

#include <string>

namespace TW::TheOpenNetwork {

class Address {
public:
    static const size_t size = Hash::sha256Size;

    std::int8_t workchainId;
    std::array<byte, size> hash{};

    /// Determines whether a string makes a valid address.
    [[nodiscard]] static bool isValid(const std::string& string) noexcept;

    /// Initializes an address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes an address with a public key.
    explicit Address(const PublicKey& publicKey, int8_t workchainId);

    /// Initializes an address with its parts
    explicit Address(int8_t workchainId, std::array<byte, size> hash)
        : workchainId(workchainId), hash(hash) {}

    /// Returns a string representation of the address.
    [[nodiscard]] std::string string() const;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.workchainId == rhs.workchainId && lhs.hash == rhs.hash;
}

} // namespace TW::TheOpenNetwork
