// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <cstddef>

#include <type_traits>

#include <TrezorCrypto/memzero.h>

namespace TW {

template <typename T>
static inline void memzero(T* data, std::size_t len = sizeof(T)) noexcept {
    static_assert(std::is_trivial_v<T>, "type should be a pod");
    ::memzero(data, len);
}

} // namespace TW
