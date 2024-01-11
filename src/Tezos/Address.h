// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../Base58Address.h"
#include "Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Tezos {

class Address : public TW::Base58Address<23> {
  public:
    /// Determines whether a string makes a valid  address.
    static bool isValid(const std::string& string);

    /// Initializes a Tezos address with a string representation.
    explicit Address(const std::string& string) : TW::Base58Address<23>(string) {}

    /// Initializes an address with a collection of bytes.
    explicit Address(const std::vector<uint8_t>& data) : TW::Base58Address<23>(data) {}

    /// Initializes a Tezos address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Derives an originated address from the given inputs.
    static std::string deriveOriginatedAddress(const std::string& operationHash, int operationIndex);

    /// Forge an address to hex bytes.
    Data forge() const;

    // without type prefix
    Data forgePKH() const;
};

} // namespace TW::Tezos
