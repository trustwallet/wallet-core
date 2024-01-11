// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../Base58Address.h"
#include "../PublicKey.h"

#include <array>
#include <cstdint>
#include <string>

namespace TW::Zcash {

class TAddress : public TW::Base58Address<22> {
  public:
    static const byte staticPrefix = 0x1c;
    static const byte p2pkh = 0xB8; // p2pkhPrefix(TWCoinType::TWCoinTypeZcash);
    static const byte p2sh = 0xBD; // p2shPrefix(TWCoinType::TWCoinTypeZcash);

    /// Determines whether a string makes a valid ZCash address.
    static bool isValid(const std::string& string) {
        return TW::Base58Address<size>::isValid(string, {{staticPrefix, p2pkh}, {staticPrefix, p2sh}});
    }

    /// Determines whether a string makes a valid ZCash address, with possible prefixes.
    static bool isValid(const std::string& string, const std::vector<Data>& validPrefixes) {
        return TW::Base58Address<size>::isValid(string, validPrefixes);
    }

    /// Initializes a  address with a string representation.
    explicit TAddress(const std::string& string) : TW::Base58Address<size>(string) {}

    /// Initializes a  address with a collection of bytes.
    explicit TAddress(const Data& data) : TW::Base58Address<size>(data) {}

    /// Initializes a  address with a public key and a prefix (2nd byte).
    TAddress(const PublicKey& publicKey, uint8_t prefix = p2pkh) : TW::Base58Address<size>(publicKey, {staticPrefix, prefix}) {}

  private:
    TAddress() = default;
};

} // namespace TW::Zcash
