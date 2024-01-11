// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
