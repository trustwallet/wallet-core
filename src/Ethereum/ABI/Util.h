// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../../uint256.h"

#include <string>

/// Some common utility methods
namespace TW::Ethereum::ABI::Util {

static constexpr size_t encodedUInt256Size = 32;

/// Compute size when padded to 32-byte boundary; round up to multiple of 32.
inline size_t paddedTo32(size_t len) { return ((len + 31) / 32) * 32; }

/// Compute padding needed to next 32-byte boundary.
inline size_t padNeeded32(size_t len) { return ((len + 31) / 32) * 32 - len; }

// uint256_t -> int256_t conversion, non trivial
inline int256_t int256FromUint256(uint256_t x)
{
    static const uint256_t maxNeg = uint256_t(1) << 255;
    if (x > maxNeg) {
        return -((int256_t)~x) - 1;
    } else {
        return (int256_t)x;
    }
}

} // namespace TW::Ethereum::ABI::Util
