// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"
#include  "../FullSS58Address.h"

#include <string>

namespace TW::Substrate {

class Address: public FullSS58Address {
  public:
    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string, int32_t network) { return FullSS58Address::isValid(string, network); }

    /// Initializes a substrate like address with a string representation.
    Address(const std::string& string, int32_t network): FullSS58Address(string, network) {}

    /// Initializes a substrate like address with a public key.
    Address(const PublicKey& publicKey, int32_t network): FullSS58Address(publicKey, network) {}
};

}

struct TWSubstrateAddress {
    TW::Substrate::Address impl;
};
