// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "AddressChecksum.h"
#include "../Bech32Address.h"

#include <string>

namespace TW::Zilliqa {

/// Zilliqa address is a Bech32Address, with "zil" prefix and Sha2 hash.
class Address : public Bech32Address {
public:
    static const std::string hrp; // HRP_ZILLIQA

    static bool isValid(const std::string& addr) { return Bech32Address::isValid(addr, hrp); }

    Address() : Bech32Address(hrp) {}

    /// Initializes an address with a key hash.
    Address(const Data& keyHash) : Bech32Address(hrp, keyHash) {}

    /// Initializes an address with a public key.
    Address(const PublicKey& publicKey) : Bech32Address(hrp, Hash::HasherSha256, publicKey) {}

    std::string checksumed() const {
        return checksum(getKeyHash());
    }

    static bool decode(const std::string& addr, Address& obj_out) {
        return Bech32Address::decode(addr, obj_out, hrp);
    }
};

} // namespace TW::Zilliqa
