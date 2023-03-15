// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"
#include "../Ethereum/Address.h"
#include <cstdint>
#include <vector>

namespace TW::Ronin {

class Address: public Ethereum::Address {
  public:
    /// Determines whether a string makes a valid  address.
    static bool isValid(const std::string& string);

    /// Initializes an address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes an address with a public key.
    explicit Address(const PublicKey& publicKey): Ethereum::Address(publicKey) {}

    /// Returns a string representation of the address.
    std::string string() const;
};

} // namespace TW::Ronin
