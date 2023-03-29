// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include <string>

namespace TW::FIO {
/// Helper class for Actor name generation from address
class Actor {
  public:
    /// Generate the actor name of the address
    static std::string actor(const Address& addr);

    /// Check if the address is valid
    static bool validate(const std::string& addr);

    /// Used internally, derive shortened uint64 key from adddr bytes
    static uint64_t shortenKey(const std::array<byte, Address::size>& addrKey);
    /// Used internally, derive name from uint64 shortened key
    static std::string name(uint64_t shortKey);
};
} // namespace TW::FIO
