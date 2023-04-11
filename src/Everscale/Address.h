// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "../PublicKey.h"

#include <optional>
#include <string>

namespace TW::Everscale {

class Address {
public:
    /// Number of bytes in an address
    static const size_t size = Hash::sha256Size;

    /// Hex address length
    static const size_t hexAddrLen = size * 2;

    /// Workchain ID (-1 for masterchain, 0 for base workchain)
    std::int8_t workchainId;
    /// StateInit hash
    std::array<byte, size> hash{};

    /// Determines whether a string makes a valid address.
    [[nodiscard]] static bool isValid(const std::string& string) noexcept;

    /// Initializes an Everscale address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes an Everscale address with a public key and a workchain id.
    explicit Address(const PublicKey& publicKey, int8_t workchainId);

    /// Initializes an Everscale address with its parts
    explicit Address(int8_t workchainId, std::array<byte, size> hash)
        : workchainId(workchainId), hash(hash) {}

    /// Returns a string representation of the address.
    [[nodiscard]] std::string string() const;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.workchainId == rhs.workchainId && lhs.hash == rhs.hash;
}

} // namespace TW::Everscale
