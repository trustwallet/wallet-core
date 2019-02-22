// Copyright © 2017-2019 Trust.
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

template <typename T, typename Data>
T load(const Data& data);

/// Loads a `int256_t` from a collection of bytes.
template <typename Data>
inline int256_t load(const Data& data) {
    int256_t result;
    import_bits(result, std::begin(data), std::end(data));
    return result;
}

/// Loads a `uint256_t` from a collection of bytes.
template <typename Data>
inline uint256_t load(const Data& data) {
    uint256_t result;
    import_bits(result, std::begin(data), std::end(data));
    return result;
}

/// Stores a `int256_t` as a collection of bytes.
inline Data store(const int256_t& v) {
    Data bytes;
    bytes.reserve(32);
    export_bits(v, std::back_inserter(bytes), 8);
    return bytes;
}

/// Stores a `uint256_t` as a collection of bytes.
inline Data store(const uint256_t& v) {
    Data bytes;
    bytes.reserve(32);
    export_bits(v, std::back_inserter(bytes), 8);
    return bytes;
}

} // namespace
