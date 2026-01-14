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
RandomResult random_buffer(uint8_t *buf, size_t len);

}
