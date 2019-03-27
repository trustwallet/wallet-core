// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

namespace TW {
namespace Hash {

/// Number of bytes in a SHA1 hash.
static const size_t sha1Size = 20;

/// Number of bytes in a SHA256 hash.
static const size_t sha256Size = 32;

/// Number of bytes in a SHA512 hash.
static const size_t sha512Size = 64;

/// Number of bytes in a RIPEMD160 hash.
static const size_t ripemdSize = 20;

/// Computes the SHA1 hash.
Data sha1(const byte* begin, const byte* end);

/// Computes the SHA256 hash.
Data sha256(const byte* begin, const byte* end);

/// Computes the SHA512 hash.
Data sha512(const byte* begin, const byte* end);

/// Computes the Keccak SHA256 hash.
Data keccak256(const byte* begin, const byte* end);

/// Computes the Keccak SHA512 hash.
Data keccak512(const byte* begin, const byte* end);

/// Computes the version 3 SHA256 hash.
Data sha3_256(const byte* begin, const byte* end);

/// Computes the version 3 SHA512 hash.
Data sha3_512(const byte* begin, const byte* end);

/// Computes the RIPEMD160 hash.
Data ripemd(const byte* begin, const byte* end);

/// Computes the Blake256 hash.
Data blake256(const byte* begin, const byte* end);

/// Computes the Blake2b hash.
Data blake2b(const byte* begin, const byte* end, size_t size);

Data blake2b(const byte* begin, const byte* end, size_t size, const Data& personal);

/// Computes the SHA1 hash.
template <typename T>
Data sha1(const T& data) {
    const auto begin = reinterpret_cast<const byte*>(data.data());
    return sha1(begin, begin + data.size());
}

/// Computes the SHA256 hash.
template <typename T>
Data sha256(const T& data) {
    const auto begin = reinterpret_cast<const byte*>(data.data());
    return sha256(begin, begin + data.size());
}

/// Computes the SHA512 hash.
template <typename T>
Data sha512(const T& data) {
    const auto begin = reinterpret_cast<const byte*>(data.data());
    return sha512(begin, begin + data.size());
}

/// Computes the Keccak SHA256 hash.
template <typename T>
Data keccak256(const T& data) {
    const auto begin = reinterpret_cast<const byte*>(data.data());
    return keccak256(begin, begin + data.size());
}

/// Computes the Keccak SHA512 hash.
template <typename T>
Data keccak512(const T& data) {
    const auto begin = reinterpret_cast<const byte*>(data.data());
    return keccak512(begin, begin + data.size());
}

/// Computes the version 3 SHA256 hash.
template <typename T>
Data sha3_256(const T& data) {
    const auto begin = reinterpret_cast<const byte*>(data.data());
    return sha3_256(begin, begin + data.size());
}

/// Computes the version 3 SHA512 hash.
template <typename T>
Data sha3_512(const T& data) {
    const auto begin = reinterpret_cast<const byte*>(data.data());
    return sha3_512(begin, begin + data.size());
}

/// Computes the RIPEMD160 hash.
template <typename T>
Data ripemd(const T& data) {
    const auto begin = reinterpret_cast<const byte*>(data.data());
    return ripemd(begin, begin + data.size());
}

/// Computes the Blake256 hash.
template <typename T>
Data blake256(const T& data) {
    const auto begin = reinterpret_cast<const byte*>(data.data());
    return blake256(begin, begin + data.size());
}

/// Computes the Blake2b hash.
template <typename T>
Data blake2b(const T& data, size_t size) {
    const auto begin = reinterpret_cast<const byte*>(data.data());
    return blake2b(begin, begin + data.size(), size);
}

template <typename T>
Data blake2b(const T& data, size_t size, const Data& personal) {
    const auto begin = reinterpret_cast<const byte*>(data.data());
    return blake2b(begin, begin + data.size(), size, personal);
}

} // namespace Hash
} // namespace TW
