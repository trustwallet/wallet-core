// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <Data.h>
#include <uint256.h>

#include <string>

namespace TW::Ethereum::EIP712 {

/// Encode a type according to EIP712, into 32 bytes.  Values are padded by 0 on the left, if not specified otherwise.
class Encoder {
public:
    static const int encodedIntSize = 32;

    static Data encodeBool(bool value);
    static Data encodeInt32(int32_t value);
    static Data encodeUInt32(uint32_t value);
    static Data encodeInt256(const int256_t& value);
    static Data encodeUInt256(const uint256_t& value);
    /// Encode the 20 bytes of an address
    static Data encodeAddress(const Data& value);
    /// Encode a string by encoding its hash
    static Data encodeString(const std::string& value);
    /// Encode a number of bytes, up to 32 bytes, padded on the right.  Longer arrays are truncated.
    static Data encodeBytes(const Data& value);
    /// Encode a dynamic number of bytes by encodign its hash
    static Data encodeBytesDyn(const Data& value);

    static uint256_t uint256FromInt256(int256_t x);
};

} // namespace TW::Ethereum::EIP712

/// Wrapper for C interface.
struct TWEthereumAEIP712Encoder {
    TW::Ethereum::EIP712::Encoder impl;
};
