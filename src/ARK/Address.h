// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../Base58Address.h"
#include "../PublicKey.h"

namespace TW::ARK {

class Address : public TW::Base58Address<21> {
  public:
    /// mainnet address prefix
    static const byte prefix = 0x17;

    /// Initializes an address with a string representation.
    explicit Address(const std::string& string) : TW::Base58Address<21>(string) {}

    /// Initializes an address with a collection of bytes.
    explicit Address(const Data& data) : TW::Base58Address<21>(data) {}

    /// Initializes an address with a public key and a prefix.
    Address(const PublicKey& publicKey);
};

} // namespace TW::ARK

// Wrapper for C interface.
struct TWARKAddress {
    TW::ARK::Address impl;
};
