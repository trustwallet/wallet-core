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

/// Loads a `uint256_t` from a collection of bytes.
template <typename D>
uint256_t load(const D& data) {
    using boost::multiprecision::cpp_int;
    uint256_t result;
    import_bits(result, std::cbegin(data), std::cend(data));
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

} // namespace
