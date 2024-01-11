// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../Base58Address.h"
#include "Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Bitcoin {

/// Class for non-segwit P2PKH and P2SH addresses
class Address : public TW::Base58Address<21> {
  public:
    /// Initializes an address with a string representation.
    explicit Address(const std::string& string) : TW::Base58Address<21>(string) {}

    /// Initializes an address with a collection of bytes.
    explicit Address(const Data& data) : TW::Base58Address<21>(data) {}

    /// Initializes an address with a public key and a prefix.  Applicable for P2PKH addresses (but not P2SH).
    Address(const PublicKey& publicKey, byte prefix) : TW::Base58Address<21>(publicKey, {prefix}) {}
};

} // namespace TW::Bitcoin

/// Wrapper for C interface.
struct TWBitcoinAddress {
    TW::Bitcoin::Address impl;
};
