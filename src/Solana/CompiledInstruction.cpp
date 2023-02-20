// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
