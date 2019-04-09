// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Base58Address.h"
#include "../Data.h"
#include "../PublicKey.h"

#include <array>
#include <string>

namespace TW::Iocoin {

class Address : public TW::Base58Address<21> {
  public:

    /// Determines whether a collection of bytes makes a valid  address.
    template <typename T>
    static bool isValid(const T& data) {
        return data.size() == size;
    }

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Determines whether a string makes a valid address, and the prefix is
    /// within the valid set.
    static bool isValid(const std::string& string, const std::vector<byte>& validPrefixes);

    /// Initializes a  address with a string representation.
    explicit Address(const std::string& string) : TW::Base58Address<21>(string) {}

    /// Initializes a  address with a public key and a prefix.
    Address(const PublicKey& publicKey, uint8_t prefix);

    /// Returns a string representation of the address.
    std::string string() const;
};
}
/// Wrapper for C interface.
struct TWIocoinAddress {
    TW::Iocoin::Address impl;
};
