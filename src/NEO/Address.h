// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Base58Address.h"
#include "../Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::NEO {

class Address : public TW::Base58Address<21> {
  public:
    /// NEO address version is 23
    /// https://github.com/neo-project/neo/blob/427a3cd08f61a33e98856e4b4312b8147708105a/neo/protocol.json#L4
    static const byte version = 0x17;

    /// Determines whether a string makes a valid NEO address.
    static bool isValid(const std::string& string);

    /// Initializes a NEO address with a string representation.
    explicit Address(const std::string& string) : TW::Base58Address<21>(string) {}

    /// Initializes a NEO address with a collection of bytes.
    explicit Address(const Data& data) : TW::Base58Address<21>(data) {}

    /// Initializes a NEO address with a public key.
    explicit Address(const PublicKey& publicKey);
};

} // namespace TW::NEO

/// Wrapper for C interface.
struct TWNEOAddress {
    TW::NEO::Address impl;
};
