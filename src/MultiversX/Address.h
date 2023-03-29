// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "../Bech32Address.h"
#include "../PublicKey.h"

#include <string>

namespace TW::MultiversX {

class Address : public Bech32Address {
public:
    /// The human-readable part of the address, as defined in "registry.json"
    static const std::string hrp; // HRP_ELROND

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    Address()
        : Bech32Address(hrp) {}

    /// Initializes an address with a key hash.
    Address(Data keyHash)
        : Bech32Address(hrp, keyHash) {}

    /// Initializes an address with a public key.
    Address(const PublicKey& publicKey)
        : Bech32Address(hrp, publicKey.bytes) {}

    static bool decode(const std::string& addr, Address& obj_out) {
        return Bech32Address::decode(addr, obj_out, hrp);
    }
};

} // namespace TW::MultiversX
