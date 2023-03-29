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

namespace TW::Tron {

class Address : public TW::Base58Address<21> {
  public:
    /// Address prefix.
    static const byte prefix = 0x41;

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Initializes an address with a string representation.
    explicit Address(const std::string& string) : TW::Base58Address<21>(string) {}

    /// Initializes an address with a collection of bytes.
    explicit Address(const Data& data) : TW::Base58Address<21>(data) {}

    /// Initializes a  address with a public key and a prefix.
    explicit Address(const PublicKey& publicKey);
};

} // namespace TW::Tron
