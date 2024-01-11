// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <algorithm>

namespace TW {

template <typename Container>
inline Container sortCopy(const Container& container) noexcept {
    Container result = container;
    std::sort(begin(result), end(result));
    return result;
}

template <typename Container, typename Func>
inline Container sortCopy(const Container& container, Func&& func) noexcept {
    Container result = container;
    std::sort(begin(result), end(result), std::forward<Func>(func));
    return result;
}

} // namespace TW