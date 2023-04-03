// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

#include <functional>

namespace TW::Hash {

/// Enum selector for the supported hash functions
enum Hasher {
    HasherSha1 = 0, // SHA1
    HasherSha256, // SHA256
    HasherSha512, // SHA512
    HasherSha512_256, // SHA512/256
    HasherKeccak256, // Keccak SHA256
    HasherKeccak512, // Keccak SHA512
    HasherSha3_256, // version 3 SHA256
    HasherSha3_512, // version 3 SHA512
    HasherRipemd, // RIPEMD160
    HasherBlake2b, // Blake2b
    HasherBlake256, // Blake256
    HasherGroestl512, // Groestl 512
    HasherSha256d, // SHA256 hash of the SHA256 hash
    HasherSha256ripemd, // ripemd hash of the SHA256 hash
    HasherSha3_256ripemd, // ripemd hash of the SHA256 hash
    HasherBlake256d, // Blake256 hash of the Blake256 hash
    HasherBlake256ripemd, // ripemd hash of the Blake256 hash
    HasherGroestl512d, // Groestl512 hash of the Groestl512 hash
};

/// Hashing function.
typedef TW::Data (*HasherSimpleType)(const TW::byte*, size_t);

/// Hash function (pointer type) from enum
TW::Hash::HasherSimpleType functionPointerFromEnum(TW::Hash::Hasher hasher);

// Digest size constants, duplicating constants from underlying lib 
/// Number of bytes in a SHA1 hash.
static const size_t sha1Size = 20;

/// Number of bytes in a SHA256 hash.
static const size_t sha256Size = 32;

/// Number of bytes in a SHA512 hash.
static const size_t sha512Size = 64;

/// Number of bytes in a RIPEMD160 hash.
static const size_t ripemdSize = 20;

/// Computes the SHA1 hash.
Data sha1(const byte* data, size_t size);

/// Computes the SHA256 hash.
Data sha256(const byte* data, size_t size);

/// Computes the SHA512 hash.
Data sha512(const byte* data, size_t size);

/// Computes the SHA512/256 hash.
Data sha512_256(const byte* data, size_t size);

/// Computes the Keccak SHA256 hash.
Data keccak256(const byte* data, size_t size);

/// Computes the Keccak SHA512 hash.
Data keccak512(const byte* data, size_t size);

/// Computes the version 3 SHA256 hash.
Data sha3_256(const byte* data, size_t size);

/// Computes the version 3 SHA512 hash.
Data sha3_512(const byte* data, size_t size);

/// Computes the RIPEMD160 hash.
Data ripemd(const byte* data, size_t size);

/// Computes the Blake256 hash.
Data blake256(const byte* data, size_t size);

/// Computes the Blake2b hash with default size (32).
Data blake2b(const byte* data, size_t dataSize);

/// Computes the Blake2b hash.
Data blake2b(const byte* data, size_t dataSize, size_t hashSize);

Data blake2b(const byte* data, size_t dataSize, size_t hsshSize, const Data& personal);

/// Computes the Groestl 512 hash.
Data groestl512(const byte* data, size_t size);

/// Computes requested hash for data (hasher enum, bytes)
inline Data hash(Hasher hasher, const byte* data, size_t dataSize) {
    const auto func = functionPointerFromEnum(hasher);
    return func(data, dataSize);
}

/// Computes requested hash for data (hasher enum)
template <typename T>
Data hash(Hasher hasher, const T& data) {
    const auto func = functionPointerFromEnum(hasher);
    return func(reinterpret_cast<const byte*>(data.data()), data.size());
}

// Templated versions for any type with data() and size()

/// Computes the SHA1 hash.
template <typename T>
Data sha1(const T& data) {
    return sha1(reinterpret_cast<const byte*>(data.data()), data.size());
}

/// Computes the SHA256 hash.
template <typename T>
Data sha256(const T& data) {
    return sha256(reinterpret_cast<const byte*>(data.data()), data.size());
}

/// Computes the SHA512 hash.
template <typename T>
Data sha512(const T& data) {
    return sha512(reinterpret_cast<const byte*>(data.data()), data.size());
}

/// Computes the SHA512/256 hash.
template <typename T>
Data sha512_256(const T& data) {
    return sha512_256(reinterpret_cast<const byte*>(data.data()), data.size());
}

/// Computes the Keccak SHA256 hash.
template <typename T>
Data keccak256(const T& data) {
    return keccak256(reinterpret_cast<const byte*>(data.data()), data.size());
}

/// Computes the Keccak SHA512 hash.
template <typename T>
Data keccak512(const T& data) {
    return keccak512(reinterpret_cast<const byte*>(data.data()), data.size());
}

/// Computes the version 3 SHA256 hash.
template <typename T>
Data sha3_256(const T& data) {
    return sha3_256(reinterpret_cast<const byte*>(data.data()), data.size());
}

/// Computes the version 3 SHA512 hash.
template <typename T>
Data sha3_512(const T& data) {
    return sha3_512(reinterpret_cast<const byte*>(data.data()), data.size());
}

/// Computes the RIPEMD160 hash.
template <typename T>
Data ripemd(const T& data) {
    return ripemd(reinterpret_cast<const byte*>(data.data()), data.size());
}

/// Computes the Blake256 hash.
template <typename T>
Data blake256(const T& data) {
    return blake256(reinterpret_cast<const byte*>(data.data()), data.size());
}

/// Computes the Blake2b hash.
template <typename T>
Data blake2b(const T& data, size_t size) {
    return blake2b(reinterpret_cast<const byte*>(data.data()), data.size(), size);
}

template <typename T>
Data blake2b(const T& data, size_t size, const Data& personal) {
    return blake2b(reinterpret_cast<const byte*>(data.data()), data.size(), size, personal);
}

/// Computes the Groestl512 hash.
template <typename T>
Data groestl512(const T& data) {
    return groestl512(reinterpret_cast<const byte*>(data.data()), data.size());
}

/// Computes the SHA256 hash of the SHA256 hash.
inline Data sha256d(const byte* data, size_t size) {
    return sha256(sha256(data, size));
}

/// Computes the ripemd hash of the SHA256 hash.
inline Data sha256ripemd(const byte* data, size_t size) {
    return ripemd(sha256(data, size));
}

/// Computes the ripemd hash of the SHA256 hash.
inline Data sha3_256ripemd(const byte* data, size_t size) {
    return ripemd(sha3_256(data, size));
}

/// Computes the Blake256 hash of the Blake256 hash.
inline Data blake256d(const byte* data, size_t size) {
    return blake256(blake256(data, size));
}

/// Computes the ripemd hash of the Blake256 hash.
inline Data blake256ripemd(const byte* data, size_t size) {
    return ripemd(blake256(data, size));
}

/// Computes the Groestl512 hash of the Groestl512 hash.
inline Data groestl512d(const byte* data, size_t size) {
    return groestl512(groestl512(data, size));
}

/// Compute the SHA256-based HMAC of a message
Data hmac256(const Data& key, const Data& message);

} // namespace TW::Hash
