// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "UInt.hpp"

#include <boost/multiprecision/cpp_int.hpp>

namespace TW {

using int256_t = boost::multiprecision::int256_t;
using uint256_t = boost::multiprecision::uint256_t;

/// Loads a `uint256_t` from a collection of bytes.
inline uint256_t load(const Data& data, int initial_position = 0) {
    return load<uint256_t>(data, initial_position);
}

/// Loads a `uint256_t` from Protobuf bytes (which are wrongly represented as
/// std::string).
inline uint256_t load(const std::string& data) {
    return load<uint256_t>(data);
}

/// Stores a `uint256_t` as a collection of bytes.
inline Data store(const uint256_t& v) {
    return store<uint256_t>(v);
}

} // namespace TW
