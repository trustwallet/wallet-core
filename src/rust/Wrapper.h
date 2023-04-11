// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <optional>

#include "Data.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"

namespace TW::Rust::internal {

/// This trait is used to constrain perfect forwarding constructors.
/// See https://rules.sonarsource.com/cpp/RSPEC-6458
template<typename Self, typename Arg>
struct IsPerfectForwarding {
    static constexpr bool value = !std::is_same_v<Self, std::remove_cv_t<typename std::remove_reference_t<Arg>>>;
};

template<typename Self, typename Arg>
inline constexpr bool IsPerfectForwardingV = IsPerfectForwarding<Self, Arg>::value;

}

namespace TW::Rust {

class CByteArrayWrapper {
public:
    CByteArrayWrapper() = default;

    /// Implicit constructor.
    CByteArrayWrapper(CByteArray&& rawArray) {
        *this = std::move(rawArray);
    }

    CByteArrayWrapper& operator=(CByteArray&& rawArray) {
        if (rawArray.data == nullptr || rawArray.size == 0) {
            return *this;
        }
        Data result(&rawArray.data[0], &rawArray.data[rawArray.size]);
        free_c_byte_array(&rawArray);
        data = std::move(result);
        return *this;
    }

    Data data;
};

class CStringWrapper {
public:
    /// Implicit move constructor.
    CStringWrapper(const char* c_str) {
        *this = c_str;
    }

    CStringWrapper& operator=(const char* c_str) {
        if (c_str == nullptr) {
            return *this;
        }
        str = c_str;
        Rust::free_string(c_str);
        return *this;
    }

    std::string str;
};

template<typename T>
class CResult {
public:
    /// Implicit move constructor.
    /// This constructor is not fired if `R` type is `CResult`, i.e not a move constructor.
    template<
        typename R,
        std::enable_if_t<internal::IsPerfectForwardingV<CResult, R>, bool> = true
    >
    CResult(R&& result) {
        *this = std::forward<R>(result);
    }

    template <typename R>
    CResult& operator=(R&& result) {
        code = result.code;
        if (code == OK_CODE) {
            inner = std::move(result.result);
        }
        return *this;
    }

    /// Returns an inner value.
    /// This method must be used if only `CResult::isOk` returns true,
    /// otherwise it throws an exception.
    T unwrap() {
        return *inner;
    }

    /// Returns the result value if `CResult::isOk` return true, otherwise returns a default value.
    T unwrap_or_default() {
        return inner ? *inner : T {};
    }

    /// Whether the result contains a value.
    bool isOk() {
        return code == OK_CODE;
    }

private:
    ErrorCode code;
    std::optional<T> inner;
};

} // namespace TW::Rust
