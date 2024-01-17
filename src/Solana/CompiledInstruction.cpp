// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Solana/CompiledInstruction.h"

namespace TW::Solana {

uint8_t CompiledInstruction::findAccount(const Address& address) {
    auto it = std::find(addresses.begin(), addresses.end(), address);
    if (it == addresses.end()) {
        throw std::invalid_argument("address not found");
    }
    assert(it != addresses.end());
    auto dist = std::distance(addresses.begin(), it);
    assert(dist < 256);
    return (uint8_t)dist;
}

}
