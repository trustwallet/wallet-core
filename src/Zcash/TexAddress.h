// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../Bech32.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Zcash {

/// A TEX Address, also called a Transparent-Source-Only Address, is a Bech32m reencoding of a transparent Zcash P2PKH address.
/// https://zips.z.cash/zip-0320#specification
class TexAddress {
    std::string hrp;

    /// Public key hash.
    Data keyHash;

public:
    static constexpr std::size_t length = 20;

    /// Determines whether a string makes a valid Bech32 address.
    static bool isValid(const std::string& addr);

    /// Determines whether a string makes a valid Bech32 address, and the HRP matches.
    static bool isValid(const std::string& addr, const std::string& hrp);

    /// Initializes an address with string representation.
    TexAddress(const std::string& addr);

    const std::string& getHrp() const { return hrp; }

    const Data& getKeyHash() const { return keyHash; }
};

}
