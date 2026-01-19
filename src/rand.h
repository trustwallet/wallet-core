// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"

namespace TW::Random {

enum RandomResult : int32_t {
    OK = 0,
    ERROR_RANDOM_GENERATION_FAILED = -1,
};

// Fills the provided buffer with cryptographically secure random bytes.
// \param data: buffer to fill with random bytes
// \param desiredSize: desired size of the buffer to be filled. If `data.size()` is different, it will be resized.
// \return RandomResult code indicating success or failure.
RandomResult random_buffer(Data& data, size_t desiredSize);

}
