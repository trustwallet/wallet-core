// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Base58Address.h"
#include "Data.h"
#include "../PublicKey.h"

#include <array>
#include <string>

namespace TW::Waves {

class Address : public Base58Address<26> {
  public:
    /// Address version.
    static const signed char v1 = 0x01;

    static const signed char mainnet = 'W';
    static const signed char testnet = 'T';

    template <typename T>
    static Data secureHash(const T &data);

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    static bool isValid(const Data& data);

    /// Initializes a  address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes a  address with a collection of bytes.
    explicit Address(const Data& data);

    /// Initializes a  address with a public key and a prefix.
    explicit Address(const PublicKey &publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
};

static inline bool operator==(const Address &lhs, const Address &rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::Waves
