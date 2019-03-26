// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include <boost/multiprecision/cpp_int.hpp>

namespace TW {

using int256_t = boost::multiprecision::int256_t;
using uint256_t = boost::multiprecision::uint256_t;

/// Loads a `uint256_t` from a collection of bytes.
inline uint256_t load(const Data& data) {
    using boost::multiprecision::cpp_int;
    if (std::empty(data)) {
        return uint256_t(0);
    }
    uint256_t result;
    import_bits(result, data.begin(), data.end());
    return result;
}

/// Loads a `uint256_t` from Protobuf bytes (which are wrongly represented as
/// std::string).
inline uint256_t load(const std::string& data) {
    using boost::multiprecision::cpp_int;
    if (std::empty(data)) {
        return uint256_t(0);
    }
    uint256_t result;
    import_bits(result, reinterpret_cast<const byte*>(data.data()),
                reinterpret_cast<const byte*>(data.data() + data.size()));
    return result;
}

/// Stores a `uint256_t` as a collection of bytes.
inline Data store(const uint256_t& v) {
    using boost::multiprecision::cpp_int;
    Data bytes;
    bytes.reserve(32);
    export_bits(v, std::back_inserter(bytes), 8);
    return bytes;
}

} // namespace TW
