// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../../uint256.h"

#include <string>
#include <cassert>

/// Some common utility methods
namespace TW::Ethereum::ABI::Util {

static constexpr size_t encodedUInt256Size = 32;

/// Compute size when padded to 32-byte boundary; round up to multiple of 32.
inline size_t paddedTo32(size_t len) { return ((len + 31) / 32) * 32; }

/// Compute padding needed to next 32-byte boundary.
inline size_t padNeeded32(size_t len) { return ((len + 31) / 32) * 32 - len; }

} // namespace TW::Ethereum::ABI::Util
