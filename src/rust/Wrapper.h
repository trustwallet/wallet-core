// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <optional>

#include "Data.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"

namespace TW::Rust {

struct CByteArrayWrapper {
    CByteArrayWrapper() = default;

    /// Implicit constructor.
    CByteArrayWrapper(const CByteArray &rawArray) {
        *this = rawArray;
    }

    CByteArrayWrapper& operator=(CByteArray rawArray) {
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

struct CStringWrapper {
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

struct CUInt64Wrapper {
    /// Implicit move constructor.
    CUInt64Wrapper(uint64_t c_u64) {
        *this = c_u64;
    }

    CUInt64Wrapper& operator=(uint64_t c_u64) {
        value = c_u64;
        return *this;
    }

    uint64_t value;
};

template<typename T>
class CResult {
public:
    /// Implicit move constructor.
    /// This constructor is not fired if `R` type is `CResult`, i.e not a move constructor.
    template<typename R>
    CResult(const R& result) {
        *this = result;
    }

    template <typename R>
    CResult& operator=(const R& result) {
        code = result.code;
        if (code == OK_CODE) {
            inner = result.result;
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
    bool isOk() const {
        return code == OK_CODE;
    }

    /// Whether the result contains an error.
    bool isErr() const {
        return !isOk();
    }

private:
    ErrorCode code;
    std::optional<T> inner;
};

using CByteArrayResultWrapper = CResult<CByteArrayWrapper>;
using CUInt64ResultWrapper = CResult<CUInt64Wrapper>;

} // namespace TW::Rust
