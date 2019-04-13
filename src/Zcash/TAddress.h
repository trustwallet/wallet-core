// Copyright © 2017-2019 Trust Wallet.
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

namespace TW::Zcash {

class TAddress : public TW::Base58Address<22> {
  public:
    static const byte staticPrefix = 0x1c;

    /// Initializes a  address with a string representation.
    explicit TAddress(const std::string& string) : TW::Base58Address<size>(string) {}

    /// Initializes a  address with a collection of bytes.
    explicit TAddress(const Data& data) : TW::Base58Address<size>(data) {}

    /// Initializes a  address with a public key and a prefix.
    TAddress(const PublicKey& publicKey, uint8_t prefix) : TW::Base58Address<size>(publicKey, {staticPrefix, prefix}) {}

  private:
    TAddress() = default;
};

} // namespace TW::Zcash

/// Wrapper for C interface.
struct TWZcashTAddress {
    TW::Zcash::TAddress impl;
};
