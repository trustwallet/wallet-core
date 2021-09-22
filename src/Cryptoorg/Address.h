// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Cosmos/Address.h"
#include "../Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Cryptoorg {

/// A Cryptoorg address is a Cosmos/Bech32 address with "cro" prefix; overridden to enforce prefix.
class Address: public Cosmos::Address {
public:
    static const std::string HRP;

    Address() : Cosmos::Address() {}

    /// Initializes an address with a key hash.
    Address(const Data& keyHash) : Cosmos::Address(HRP, keyHash) {}

    /// Initializes an address with a public key.
    Address(const PublicKey& publicKey) : Cosmos::Address(HRP, publicKey) {}

    /// Creates an address object from the given string, if valid.  Returns success.
    static bool decode(const std::string& addr, Address& obj_out) {
        return Bech32Address::decode(addr, obj_out, HRP);
    }

    /// Creates an address object from the given string, if valid.  Returns success.
    static bool isValid(const std::string& addr) {
        return Bech32Address::isValid(addr, HRP);
    }
};

} // namespace TW::Cryptoorg
