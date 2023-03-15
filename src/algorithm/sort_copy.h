// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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