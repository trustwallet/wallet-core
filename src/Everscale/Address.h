// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include <optional>
#include <string>

namespace TW::Everscale {

class Address {
public:
    /// Number of bytes in an address
    static const size_t size = 32;

    static const std::int8_t basechain_id = 0;
    static const std::int8_t masterchain_id = -1;

    std::int32_t workchain_id;
    std::array<byte, size> bytes;

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Initializes a Everscale address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes a Everscale address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
private:
    static std::optional<std::pair<int8_t, int32_t>> parseWorkchainId(const std::string& string);
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.workchain_id == rhs.workchain_id && lhs.bytes == rhs.bytes;
}

} // namespace TW::Everscale
