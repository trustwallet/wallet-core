// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Base58Address.h"
#include "../PublicKey.h"

#include <array>
#include <cstdint>
#include <string>

namespace TW::Nebl {

class TAddress : public TW::Base58Address<21> {
  public:
    static const TW::byte p2pkh = 0x35; // p2pkhPrefix Neblio: addresses begin with 'N'
    static const TW::byte p2sh = 0x70; // p2shPrefix Neblio: addresses begin with 'n'

    // static const TW::byte p2pkh = 0x41; // testnet
    // static const TW::byte p2sh = 0x7f; // testnet


    /// Determines whether a string makes a valid Nebl address.
    static bool isValid(const std::string& string) {
        return TW::Base58Address<size>::isValid(string, {{p2pkh}, {p2sh}});
    }

    /// Determines whether a string makes a valid Nebl address, with possible prefixes.
    static bool isValid(const std::string& string, const std::vector<Data>& validPrefixes) {
        return TW::Base58Address<size>::isValid(string, validPrefixes);
    }

    /// Initializes a  address with a string representation.
    explicit TAddress(const std::string& string) : TW::Base58Address<size>(string) {}

    /// Initializes a  address with a collection of bytes.
    explicit TAddress(const Data& data) : TW::Base58Address<size>(data) {}

    /// Initializes a  address with a public key and a prefix (2nd byte).
    TAddress(const PublicKey& publicKey, uint8_t prefix = p2pkh) : TW::Base58Address<size>(publicKey, {prefix}) {}

  private:
    TAddress() = default;
};

} // namespace TW::Nebl
