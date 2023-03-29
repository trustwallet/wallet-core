// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <Data.h>
#include <uint256.h>

#include <string>

namespace TW::Ethereum::ABI {

/// Encode a type according to EIP712, into 32 bytes.
/// Result is appended to the provided Data object.
/// Values are padded by 0 on the left -- unless otherwise mentioned.
/// See: https://github.com/ethereum/EIPs/blob/master/EIPS/eip-712.md
class ValueEncoder {
public:
    static const int encodedIntSize = 32;

    static void encodeBool(bool value, Data& inout);
    static void encodeInt32(int32_t value, Data& inout);
    static void encodeUInt32(uint32_t value, Data& inout);
    static void encodeInt256(const int256_t& value, Data& inout);
    static void encodeUInt256(const uint256_t& value, Data& inout);
    /// Encode the 20 bytes of an address
    static void encodeAddress(const Data& value, Data& inout);
    /// Encode a string by encoding its hash
    static void encodeString(const std::string& value, Data& inout);
    /// Encode a number of bytes, up to 32 bytes, padded on the right.  Longer arrays are truncated.
    static void encodeBytes(const Data& value, Data& inout);
    /// Encode a dynamic number of bytes by encoding its hash
    static void encodeBytesDyn(const Data& value, Data& inout);

    // uint256_t -> int256_t conversion, non trivial
    static int256_t int256FromUint256(uint256_t x);
    static uint256_t uint256FromInt256(int256_t x);

    /// Compute size when padded to 32-byte boundary; round up to multiple of 32.
    static inline size_t paddedTo32(size_t len) { return ((len + 31) / 32) * 32; }

    /// Compute padding needed to next 32-byte boundary.
    static inline size_t padNeeded32(size_t len) { return ((len + 31) / 32) * 32 - len; }
};
} // namespace TW::Ethereum::ABI
