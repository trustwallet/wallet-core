// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "rand.h"
#include "rust/Wrapper.h"

namespace TW::Random {

RandomResult random_buffer(uint8_t *buf, size_t len) {
    const int32_t res = Rust::crypto_random_buffer(buf, len);
    if (res == static_cast<int32_t>(Rust::OK_CODE)) {
        return OK;
    }

    return ERROR_RANDOM_GENERATION_FAILED;
}

} // namespace TW::Random
