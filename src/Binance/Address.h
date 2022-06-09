// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bech32Address.h"

#include <string>

namespace TW::Binance {

/// Binance address is a Bech32Address, with "bnb" prefix and sha256ripemd hash
class Address: public Bech32Address {
public:
    static const std::string hrp; // HRP_BINANCE
    static const std::string hrpValidator; // HRP_BINANCE

    static bool isValid(const std::string& addr);

    Address() : Bech32Address(hrp) {}

    /// Initializes an address with a key hash.
    Address(const Data& keyHash) : Bech32Address(hrp, keyHash) {}

    /// Initializes an address with a public key.
    Address(const PublicKey& publicKey) : Bech32Address(hrp, Hash::HasherSha256ripemd, publicKey) {}

    static bool decode(const std::string& addr, Address& obj_out);
};

} // namespace TW::Binance
