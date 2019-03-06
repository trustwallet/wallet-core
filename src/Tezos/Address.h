// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include <string>
#include <vector>

namespace TW {
namespace Tezos {

class Address {
public:
    static const size_t addressSize = 27;
    static const size_t checksumLength = 4;
    /// Number of bytes in an address.
    static const size_t size = 23;
    std::array<byte, size> bytes;
    std::string public_key_hash;

    /// Initializes a Tezos address with a public_key hash.
    Address(const std::string& string);
    Address(const std::vector<uint8_t>& data);
    Address(const PublicKey& publicKey);
    static bool isValid(const std::string& string);
    /// Returns a string representation of the address.
    std::string string() const;
};

}} // namespace
