// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <random>

extern "C" {
uint32_t random32(void) {
    std::mt19937 rng(std::random_device{}());
    return rng();
}

void random_buffer(uint8_t* buf, size_t len) {
    std::mt19937 rng(std::random_device{}());
    std::generate_n(buf, len, [&rng]() -> uint8_t { return rng() & 0x000000ff; });
    return;
}

} // extern "C"
