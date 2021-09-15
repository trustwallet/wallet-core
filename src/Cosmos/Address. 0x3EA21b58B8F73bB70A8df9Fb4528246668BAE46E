// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bech32Address.h"
#include "../Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Cosmos {

/// A Bech32 Cosmos address.  Hrp has to be specified (e.g. "cosmos", "terra"...), hash is HASHER_SHA2_RIPEMD.
class Address: public Bech32Address {
public:
    Address() : Bech32Address("") {}

    /// Initializes an address with a key hash.
    Address(const std::string& hrp, const Data& keyHash) : Bech32Address(hrp, keyHash) {}

    /// Initializes an address with a public key.
    Address(const std::string& hrp, const PublicKey& publicKey) : Bech32Address(hrp, HASHER_SHA2_RIPEMD, publicKey) {}

    /// Creates an address object from the given string, if valid.  Returns success.
    static bool decode(const std::string& addr, Address& obj_out) {
        return Bech32Address::decode(addr, obj_out, "");
    }
};

} // namespace TW::Cosmos
