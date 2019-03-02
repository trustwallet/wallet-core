// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <numeric>

namespace TW {
namespace Ethereum {

template <typename T>
bool is_dynamic(std::vector<T>) {
    return true;
}

template <typename T>
std::size_t size(const std::vector<T>& array) {
    return 32 + std::accumulate(array.begin(), array.end(), 0u, [](size_t sum, auto x) { return sum + size(x); });
}

template <typename T>
void encode(const std::vector<T>& array, Data& data) {
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
std::string type_string(const std::vector<T>& array) {
    return type_string(array[0]) + "[]";
}

}} // namespace
