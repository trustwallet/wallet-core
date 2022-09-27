// Copyright © 2017-2022 Trust Wallet.
// Author: Clement Doumergue
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
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

    /// Initializes an Aptos address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes an Aptos address with a collection of bytes
    explicit Address(const Data& data);

    /// Initializes an Aptos address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    [[nodiscard]] std::string string(bool withPrefix = true) const;
};

inline const Address gAddressZero = Address("0x0");
inline const Address gAddressOne = Address("0x1");

} // namespace TW::Aptos
