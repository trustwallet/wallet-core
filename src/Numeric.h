// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <numeric>

namespace TW {

template <
    typename T,
    typename = std::enable_if_t<std::is_unsigned_v<T>>
>
bool checkAddUnsignedOverflow(T x, T y) {
    return x > std::numeric_limits<T>::max() - y;
}

template <
    typename T,
    typename = std::enable_if_t<std::is_unsigned_v<T>>
>
bool checkMulUnsignedOverflow(T x, T y) {
    if (x == 0 || y == 0) {
        return false;
    }
    return x > std::numeric_limits<T>::max() / y;
}

} // namespace TW
