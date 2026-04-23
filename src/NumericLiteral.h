// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <cstddef> // std::size_t
#include <cstdint>

// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p0330r8.html
inline constexpr std::size_t operator"" _uz(unsigned long long int n) {
    return n;
}

inline constexpr std::uint64_t operator"" _u64(std::uint64_t n) {
    return n;
}