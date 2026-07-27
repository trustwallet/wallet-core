// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <cstddef>

#include <type_traits>
#include <utility>

#include "Data.h"
#include <TrezorCrypto/memzero.h>

namespace TW {

template <typename T>
static inline void memzero(T* data, std::size_t len = sizeof(T)) noexcept {
    static_assert(std::is_trivial_v<T>, "type should be a pod");
    ::memzero(data, len);
}

/// RAII wrapper that securely zeroizes its contents on destruction.
/// Works with any type that exposes `.data()`, `.size()`, and `.empty()`,
/// e.g. `Data` (std::vector<byte>) and `std::string`.
template <typename T>
class Zeroizing {
    static_assert(sizeof(typename T::value_type) == 1,
                  "Zeroizing requires a byte-sized element type; otherwise size() != byte count");
    static_assert(std::is_trivial_v<typename T::value_type>, "type should be a pod");

public:
    Zeroizing() = default;

    explicit Zeroizing(T&& v) : value_(std::move(v)) {}

    // Copying sensitive material is explicitly disallowed.
    Zeroizing(const Zeroizing&) = delete;
    Zeroizing& operator=(const Zeroizing&) = delete;

    ~Zeroizing() { zeroize(); }

    T& get() noexcept {
        return value_;
    }

    const T& get() const noexcept {
        return value_;
    }

private:
    void zeroize() noexcept {
        if (!value_.empty()) {
            TW::memzero(value_.data(), value_.size());
        }
    }

    T value_;
};

using ZeroizingData = Zeroizing<Data>;
using ZeroizingString = Zeroizing<std::string>;

} // namespace TW
