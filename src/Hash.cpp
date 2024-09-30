// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Hash.h"

#include "rust/bindgen/WalletCoreRSBindgen.h"
#include "rust/Wrapper.h"
#include <string>

using namespace TW;

TW::Hash::HasherSimpleType Hash::functionPointerFromEnum(TW::Hash::Hasher hasher) {
    switch (hasher) {
    case Hash::HasherSha1:
        return Hash::sha1;
    case Hash::HasherSha256:
        return Hash::sha256;
    case Hash::HasherSha512:
        return Hash::sha512;
    case Hash::HasherSha512_256:
        return Hash::sha512_256;
    case Hash::HasherKeccak256:
        return Hash::keccak256;
    case Hash::HasherKeccak512:
        return Hash::keccak512;
    case Hash::HasherSha3_256:
        return Hash::sha3_256;
    case Hash::HasherSha3_512:
        return Hash::sha3_512;
    case Hash::HasherRipemd:
        return Hash::ripemd;
    case Hash::HasherBlake256:
        return Hash::blake256;
    case Hash::HasherGroestl512:
        return Hash::groestl512;
    case Hash::HasherSha256d:
        return Hash::sha256d;
    case Hash::HasherSha256ripemd:
        return Hash::sha256ripemd;
    case Hash::HasherSha3_256ripemd:
        return Hash::sha3_256ripemd;
    case Hash::HasherBlake2b:
        return Hash::blake2b;
    case Hash::HasherBlake256d:
        return Hash::blake256d;
    case Hash::HasherBlake256ripemd:
        return Hash::blake256ripemd;
    case Hash::HasherGroestl512d:
        return Hash::groestl512d;
    }
}

Data Hash::sha1(const byte* data, size_t size) {
    return Rust::CByteArrayWrapper(Rust::sha1(data, size)).data;
}

Data Hash::sha256(const byte* data, size_t size) {
    return Rust::CByteArrayWrapper(Rust::sha256(data, size)).data;
}

Data Hash::sha512(const byte* data, size_t size) {
    return Rust::CByteArrayWrapper(Rust::sha512(data, size)).data;
}

Data Hash::sha512_256(const byte* data, size_t size) {
    return Rust::CByteArrayWrapper(Rust::sha512_256(data, size)).data;
}

Data Hash::keccak256(const byte* data, size_t size) {
    return Rust::CByteArrayWrapper(Rust::keccak256(data, size)).data;
}

Data Hash::keccak512(const byte* data, size_t size) {
    return Rust::CByteArrayWrapper(Rust::keccak512(data, size)).data;
}

Data Hash::sha3_256(const byte* data, size_t size) {
    return Rust::CByteArrayWrapper(Rust::sha3__256(data, size)).data;
}

Data Hash::sha3_512(const byte* data, size_t size) {
    return Rust::CByteArrayWrapper(Rust::sha3__512(data, size)).data;
}

Data Hash::ripemd(const byte* data, size_t size) {
    return Rust::CByteArrayWrapper(Rust::ripemd_160(data, size)).data;
}

Data Hash::blake256(const byte* data, size_t size) {
    return Rust::CByteArrayWrapper(Rust::blake_256(data, size)).data;
}

Data Hash::blake2b(const byte* data, size_t dataSize) {
    Rust::CByteArrayResultWrapper res = Rust::blake2_b(data, dataSize, 32);
    if (res.isErr()) {
        throw std::runtime_error("Error 'blake2_b' hashing");
    }
    return res.unwrap().data;
}

Data Hash::blake2b(const byte* data, size_t dataSize, size_t hashSize) {
    Rust::CByteArrayResultWrapper res = Rust::blake2_b(data, dataSize, hashSize);
    if (res.isErr()) {
        throw std::runtime_error("Error 'blake2_b' hashing");
    }
    return res.unwrap().data;
}

Data Hash::blake2b(const byte* data, size_t dataSize, size_t hashSize, const Data& personal) {
    Rust::CByteArrayResultWrapper res = Rust::blake2_b_personal(data, dataSize, hashSize, personal.data(), personal.size());
    if (res.isErr()) {
        throw std::runtime_error("Error 'blake2_b_personal' hashing");
    }
    return res.unwrap().data;
}

Data Hash::groestl512(const byte* data, size_t size) {
    return Rust::CByteArrayWrapper(Rust::groestl_512(data, size)).data;
}

Data Hash::hmac256(const Data& key, const Data& message) {
    Rust::CByteArrayWrapper res = Rust::hmac__sha256(key.data(), key.size(), message.data(), message.size());
    return res.data;
}
