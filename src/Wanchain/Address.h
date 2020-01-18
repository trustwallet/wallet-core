// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Ethereum/Address.h"
#include "../PublicKey.h"

#include <array>
#include <cstdint>
#include <vector>

namespace TW::Wanchain {
class Address : public Ethereum::Address {
  public:
    using Ethereum::Address::Address;

    /// Returns a string representation of the address.
    std::string string() const;
};
} // namespace TW::Wanchain
