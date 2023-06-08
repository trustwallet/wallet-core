// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
