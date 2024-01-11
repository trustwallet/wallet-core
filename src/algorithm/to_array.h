// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <algorithm>
#include <array>

namespace TW {

template <typename T, std::size_t N, typename Collection>
constexpr std::array<T, N> to_array(Collection&& collection) {
    std::array<T, N> out{};
    std::copy(begin(collection), end(collection), out.begin());
    return out;
}

} // namespace TW
