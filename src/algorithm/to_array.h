// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
