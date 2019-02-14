// Copyright © 2017-2019 Trust.
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
template<typename T>
Data sha1(const T& data);

/// Computes the SHA256 hash.
template<typename T>
Data sha256(const T& data);

/// Computes the SHA512 hash.
template<typename T>
Data sha512(const T& data);

/// Computes the Keccak SHA256 hash.
template<typename T>
Data keccak256(const T& data);

/// Computes the Keccak SHA512 hash.
template<typename T>
Data keccak512(const T& data);

/// Computes the version 3 SHA256 hash.
template<typename T>
Data sha3_256(const T& data);

/// Computes the version 3 SHA512 hash.
template<typename T>
Data sha3_512(const T& data);

/// Computes the RIPEMD160 hash.
template<typename T>
Data ripemd(const T& data);

/// Computes the Blake2b hash.
template<typename T>
Data blake2b(const T& data, size_t size);

template<typename T>
Data blake2b(const T& data, size_t size, const Data& personal);

}} // namespace
