// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "rand.h"
#include "rust/Wrapper.h"

namespace TW::Random {

RandomResult random_buffer(Data& data, size_t desiredSize) {
    if (data.size() != desiredSize) {
        data.resize(desiredSize);
    }
    const int32_t res = Rust::crypto_random_buffer(data.data(), data.size());
    if (res == static_cast<int32_t>(Rust::OK_CODE)) {
        return OK;
    }

    return ERROR_RANDOM_GENERATION_FAILED;
}

} // namespace TW::Random
