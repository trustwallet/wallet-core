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

namespace bm = boost::multiprecision;

using int160_t = bm::number<bm::backends::cpp_int_backend<160, 160, bm::signed_magnitude, bm::unchecked, void>>;
using uint160_t = bm::number<bm::backends::cpp_int_backend<160, 160, bm::unsigned_magnitude, bm::unchecked, void>>;

template<class T>
inline T load(const Data &data, int initial_position = 0) {
    using bm::cpp_int;
    if (data.empty()) {
        return (T)(0);
    }
    T result;
    import_bits(result, data.begin() + initial_position, data.end());
    return result;
}

template<class T>
inline T load(const std::string &data) {
    using bm::cpp_int;
    if (data.empty()) {
        return (T)(0);
    }
    T result;
    import_bits(result, reinterpret_cast<const byte *>(data.data()),
                reinterpret_cast<const byte *>(data.data() + data.size()));
    return result;
}

template<class T>
inline Data store(const T &v) {
    using bm::cpp_int;
    Data bytes;
    bytes.reserve(20);
    export_bits(v, std::back_inserter(bytes), 8);
    return bytes;
}

} // namespace TW
