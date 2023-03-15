// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
    // Decode an array of given simple types; return each element as a string in a vector
    static std::vector<std::string> decodeArray(const Data& data, const std::string& elementType);
};
} // namespace TW::Ethereum::ABI
