// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"

#include <array>
#include <stdint.h>
#include <vector>

namespace TW {
namespace Nimiq {

class Address {
public:
    /// Number of bytes in an address.
    static const size_t size = 20;

    /// Address data consisting of a prefix byte followed by the public key hash.
    std::array<uint8_t, size> bytes;

    /// Determines whether a collection of bytes makes a valid  address.
    static bool isValid(const std::vector<uint8_t>& data) {
        return data.size() == size;
    }

    /// Determines whether a string makes a valid  address.
    static bool isValid(const std::string& string);

    /// Initializes an address with a string representation.
    Address(const std::string& string);

    /// Initializes an address with a collection of bytes.
    Address(const std::vector<uint8_t>& data);

    /// Initializes an address with a public key.
    Address(const std::array<uint8_t, 32> publicKey);

    /// Returns a string representation of the address.
    std::string string() const;

private:
    int getChecksum() const;
};

static inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}
    
}} // namespace
