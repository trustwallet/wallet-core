// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_int/cpp_int_config.hpp>

namespace TW {

using int160_t = number<boost::multiprecision::backends::cpp_int_backend<160, 160, boost::multiprecision::signed_magnitude, boost::multiprecision::unchecked, void>>;
using uint160_t = number<boost::multiprecision::backends::cpp_int_backend<160, 160, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void>>;

/// Loads a `uint160_t` from a collection of bytes.
inline uint160_t load160(const Data &data, int initial_position = 0) {
    using boost::multiprecision::cpp_int;
    if (data.empty()) {
        return uint160_t(0);
    }
    uint160_t result;
    import_bits(result, data.begin() + initial_position, data.end());
    return result;
}

/// Loads a `uint160_t` from Protobuf bytes (which are wrongly represented as
/// std::string).
inline uint160_t load160(const std::string &data) {
    using boost::multiprecision::cpp_int;
    if (data.empty()) {
        return uint160_t(0);
    }
    uint160_t result;
    import_bits(result, reinterpret_cast<const byte *>(data.data()),
                reinterpret_cast<const byte *>(data.data() + data.size()));
    return result;
}

/// Stores a `uint160_t` as a collection of bytes.
inline Data store(const uint160_t &v) {
    using boost::multiprecision::cpp_int;
    Data bytes;
    bytes.reserve(20);
    export_bits(v, std::back_inserter(bytes), 8);
    return bytes;
}

} // namespace TW
