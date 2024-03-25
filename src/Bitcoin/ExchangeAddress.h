// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../Base58Address.h"
#include "Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Bitcoin {

// see: https://github.com/firoorg/firo/blob/8bd4abdea223e22f15c36e7d2d42618dc843e2ef/src/chainparams.cpp#L357
static const size_t kExchangeAddressSize = 23;
static const Data kPrefix = {0x01, 0xb9, 0xbb};

/// Class for firo exchange addresses
class ExchangeAddress : public TW::Base58Address<kExchangeAddressSize> {
  public:
    /// Initializes an address with a string representation.
    explicit ExchangeAddress(const std::string& string) : TW::Base58Address<kExchangeAddressSize>(string) {}

    /// Initializes an address with a collection of bytes.
    explicit ExchangeAddress(const Data& data) : TW::Base58Address<kExchangeAddressSize>(data) {}

    /// Initializes an address with a public key and prefix.
    ExchangeAddress(const PublicKey& publicKey) : TW::Base58Address<kExchangeAddressSize>(publicKey, kPrefix) {}

    /// Determines whether a string makes a valid Firo exchange address.
    static bool isValid(const std::string& string) {
        return TW::Base58Address<size>::isValid(string, {kPrefix});
    }
};

} // namespace TW::Bitcoin
