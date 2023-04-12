// Copyright © 2017-2022 Trust Wallet.
// Author: Clement Doumergue
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "BCS.h"
#include "Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Aptos {

class Address {
public:
    static constexpr size_t size = 32;

    std::array<byte, size> bytes;

    /// Determines whether a collection of bytes makes a valid address.
    static bool isValid(const Data& data) { return data.size() == size; }

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    static Address zero() {
        return Address("0x0");
    }

    static Address one() {
        return Address("0x1");
    }

    static Address three() {
        return Address("0x3");
    }

    /// Initializes an Aptos address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes an Aptos address with a collection of bytes
    explicit Address(const Data& data);

    /// Initializes an Aptos address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Constructor that allow factory programming;
    Address() noexcept = default;

    /// Returns a string representation of the address.
    [[nodiscard]] std::string string(bool withPrefix = true) const;

    /// Returns a short string representation of the address. E.G 0x1;
    [[nodiscard]] std::string shortString() const;
};

constexpr inline bool operator==(const Address& lhs, const Address& rhs) noexcept {
    return lhs.bytes == rhs.bytes;
}

BCS::Serializer& operator<<(BCS::Serializer& stream, Address) noexcept;

} // namespace TW::Aptos
