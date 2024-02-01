// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <Data.h>
#include <uint256.h>

#include <vector>

namespace TW::Ethereum::ABI {

class ValueDecoder {
public:
    // Decode a UInt256 value
    static uint256_t decodeUInt256(const Data& data);
    // Decode an arbitrary type, return value as string
    static std::string decodeValue(const Data& data, const std::string& type);
};
} // namespace TW::Ethereum::ABI
