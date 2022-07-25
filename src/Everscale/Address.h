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
    static const size_t size = Hash::sha256Size;

    using MsgAddressInt = std::pair<int8_t, std::array<byte, Address::size>>;

    std::int8_t wc_;
    std::array<byte, size> address_;

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Initializes a Everscale address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes a Everscale address with a public key and workchain id.
    explicit Address(const PublicKey& publicKey, int8_t workchainId);

    /// Returns a string representation of the address.
    std::string string() const;
private:
    static std::optional<std::pair<int8_t, int32_t>> parseWorkchainId(const std::string& string);

};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.wc_ == rhs.wc_ && lhs.address_ == rhs.address_;
}

} // namespace TW::Everscale
