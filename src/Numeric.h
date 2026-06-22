// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <numeric>
#include <stdexcept>

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

template <
    typename T,
    typename = std::enable_if_t<std::is_unsigned_v<T>>
>
T addUnsignedChecked(T x, T y) {
    if (checkAddUnsignedOverflow(x, y)) {
        throw std::overflow_error("Unsigned integer addition overflow");
    }
    return x + y;
}

template <
    typename T,
    typename = std::enable_if_t<std::is_unsigned_v<T>>
>
T mulUnsignedChecked(T x, T y) {
    if (checkMulUnsignedOverflow(x, y)) {
        throw std::overflow_error("Unsigned integer multiplication overflow");
    }
    return x * y;
}

} // namespace TW
