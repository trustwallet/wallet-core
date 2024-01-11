// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <optional>
#include <string>
#include <type_traits>

namespace TW {

namespace Types {
template <typename T>
struct Success {
    Success(const T& val) : val(val) {}
    Success(T&& val) : val(std::move(val)) {}

    T val;
};

template <>
struct Success<void> {};

template <typename E>
struct Failure {
    Failure(const E& val) : val(val) {}
    Failure(E&& val) : val(std::move(val)) {}

    E val;
};
} // namespace Types

template <typename T, typename E = std::string>
struct Result {
  private:
    static_assert(!std::is_same<E, void>::value, "void error type is not allowed");
    static constexpr size_t Size = sizeof(T) > sizeof(E) ? sizeof(T) : sizeof(E);
    static constexpr size_t Align = sizeof(T) > sizeof(E) ? alignof(T) : alignof(E);
    using Storage = typename std::aligned_storage<Size, Align>::type;

    /// Wether the operation succeeded.
    bool success_{false};
    Storage storage_;

  public:
    /// Initializes a success result with a payload.
    Result(Types::Success<T> payload) : success_(true) { new (&storage_) T(payload.val); }

    /// Initializes a failure result.
    Result(Types::Failure<E> error) : success_(false) { new (&storage_) E(error.val); }

    Result(const Result& other) : success_(other.success_) {
        if (success_) {
            new (&storage_) T(other.get<T>());
        } else {
            new (&storage_) E(other.get<E>());
        }
    }

    Result& operator=(const Result& other) {
        if (success_) {
            get<T>().~T();
        } else {
            get<E>().~E();
        }

        success_ = other.success_;
        if (success_) {
            new (&storage_) T(other.get<T>());
        } else {
            new (&storage_) E(other.get<E>());
        }
        return *this;
    }

    Result(Result&& other) {
        if (success_) {
            new (&storage_) T(other.get<T>());
        } else {
            new (&storage_) E(other.get<E>());
        }
    }

    Result& operator=(Result&& other) {
        if (success_) {
            get<T>().~T();
        } else {
            get<E>().~E();
        }

        success_ = other.success_;
        if (success_) {
            new (&storage_) T(std::move(other.get<T>()));
        } else {
            new (&storage_) E(std::move(other.get<E>()));
        }
        return *this;
    }

    ~Result() {
        if (success_)
            get<T>().~T();
        else
            get<E>().~E();
    }

    bool isSuccess() const { return success_; }

    bool isFailure() const { return !success_; }

    /// Returns the contained payload.
    ///
    /// The behavior is undefined if this result is a failure.
    T payload() const { return get<T>(); }

    /// Returns the contained error.
    ///
    /// The behavior is undefined if this result is a success.
    E error() const { return get<E>(); }

    /// Returns a new success result with the given payloadd.
    static Result<T, E> success(T&& val) { return Result(Types::Success<T>(std::move(val))); }

    /// Returns a new failure result with the given error.
    static Result<T, E> failure(E&& val) { return Result(Types::Failure<E>(std::move(val))); }

    static Result<T, E> failure(E& val) { return Result(Types::Failure<E>(val)); }

    operator bool() const { return success_; }

  private:
    template <typename U>
    const U& get() const {
        return *reinterpret_cast<const U*>(&storage_);
    }

    template <typename U>
    U& get() {
        return *reinterpret_cast<U*>(&storage_);
    }
};

template <typename E>
struct Result<void, E> {
  private:
    /// Wether the operation succeeded.
    bool success_;
    std::optional<E> error_;

  public:
    /// Initializes a success result with a payload.
    Result([[maybe_unused]] Types::Success<void> payload) : success_(true), error_() {}

    /// Initializes a failure result.
    Result(Types::Failure<E> error) : success_(false), error_(error.val) {}

    bool isSuccess() const { return success_; }

    bool isFailure() const { return !success_; }

    /// Returns the contained error.
    ///
    /// The behavior is undefined if this result is a success.
    E error() const { return *error_; }

    /// Returns a new success result with no payloadd.
    static inline Result<void, E> success() { return Result(Types::Success<void>()); }

    /// Returns a new failure result with the given error.
    static Result<void, E> failure(E&& val) {
        return Result(Types::Failure<E>(std::move(val)));
    }

    operator bool() const { return success_; }
};

} // namespace TW
