// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../../Data.h"
#include "../../uint256.h"
#include "Numbers.h"
#include "Bytes.h"

#include <numeric>

namespace TW::Ethereum {

template <typename T>
inline bool is_dynamic(std::vector<T>) {
    return true;
}

template <typename T>
inline std::size_t size(const std::vector<T>& array) {
    size_t rest = std::accumulate(array.begin(), array.end(), 0u,
                                [](size_t sum, auto x) { return sum + size(x); });
    return 32 + paddedTo32(rest);
}

template <typename T>
inline void encode(const std::vector<T>& array, Data& data) {
    encode(uint256_t(array.size()), data);

    std::size_t headSize = 0;
    for (auto& x : array) {
        if (is_dynamic(x)) {
            headSize += 32;
        } else {
            headSize += size(x);
        }
    }

    std::size_t dynamicOffset = 0;
    for (auto& x : array) {
        if (is_dynamic(x)) {
            encode(uint256_t(headSize + dynamicOffset), data);
            dynamicOffset += size(x);
        } else {
            encode(x, data);
        }
    }

    for (auto& x : array) {
        if (is_dynamic(x))
            encode(x, data);
    }
}

template <typename T>
inline std::string type_string(const std::vector<T>& array) {
    return type_string(array[0]) + "[]";
}

template <typename T>
inline bool decode(const Data& encoded, std::vector<T>& decoded, size_t& offset_inout) {
    size_t origOffset = offset_inout;
    // read length
    uint256_t len256;
    if (!decode(encoded, len256, offset_inout)) { return false; }
    // check if length is in the size_t range
    size_t len = static_cast<size_t>(len256);
    if (len256 != static_cast<uint256_t>(len)) { return false; }
    // read values
    decoded = std::vector<T>(len);
    for (int i = 0; i < len; ++i) {
        if (!decode(encoded, decoded[i], offset_inout)) { return false; }
    }
    // padding
    offset_inout = origOffset + paddedTo32(offset_inout - origOffset);
    // TODO handle dynamic types
    return true;
}

} // namespace TW::Ethereum
