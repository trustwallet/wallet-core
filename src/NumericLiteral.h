// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <cstddef> // std::size_t

// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p0330r8.html
inline constexpr std::size_t operator"" _uz(unsigned long long int n) {
    return n;
}