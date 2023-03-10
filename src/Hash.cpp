// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Hash.h"
#include "BinaryCoding.h"

#include "rust/bindgen/RAII.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"
#include <string>

using namespace TW;

TW::Hash::HasherSimpleType Hash::functionPointerFromEnum(TW::Hash::Hasher hasher) {
    switch (hasher) {
    case Hash::HasherSha1:
        return Hash::sha1;
    default:
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
    case Hash::HasherBlake256d:
        return Hash::blake256d;
    case Hash::HasherBlake256ripemd:
        return Hash::blake256ripemd;
    case Hash::HasherGroestl512d:
        return Hash::groestl512d;
    }
}

Data Hash::sha1(const byte* data, size_t size) {
    return Rust::data_from_c_byte_array(Rust::sha1(data, size));
}

Data Hash::sha256(const byte* data, size_t size) {
    return Rust::data_from_c_byte_array(Rust::sha256(data, size));
}

Data Hash::sha512(const byte* data, size_t size) {
    return Rust::data_from_c_byte_array(Rust::sha512(data, size));
}

Data Hash::sha512_256(const byte* data, size_t size) {
    return Rust::data_from_c_byte_array(Rust::sha512_256(data, size));
}

Data Hash::keccak256(const byte* data, size_t size) {
    return Rust::data_from_c_byte_array(Rust::keccak256(data, size));
}

Data Hash::keccak512(const byte* data, size_t size) {
    return Rust::data_from_c_byte_array(Rust::keccak512(data, size));
}

Data Hash::sha3_256(const byte* data, size_t size) {
    return Rust::data_from_c_byte_array(Rust::sha3__256(data, size));
}

Data Hash::sha3_512(const byte* data, size_t size) {
    return Rust::data_from_c_byte_array(Rust::sha3__512(data, size));
}

Data Hash::ripemd(const byte* data, size_t size) {
    return Rust::data_from_c_byte_array(Rust::ripemd_160(data, size));
}

Data Hash::blake256(const byte* data, size_t size) {
    return Rust::data_from_c_byte_array(Rust::blake_256(data, size));
}

Data Hash::blake2b(const byte* data, size_t dataSize, size_t hashSize) {
    return Rust::data_from_c_byte_array(Rust::blake2_b(data, dataSize, hashSize));
}

Data Hash::blake2b(const byte* data, size_t dataSize, size_t hashSize, const Data& personal) {
    return Rust::data_from_c_byte_array(Rust::blake2_b_personal(data, dataSize, hashSize, personal.data(), personal.size()));
}

Data Hash::groestl512(const byte* data, size_t size) {
    return Rust::data_from_c_byte_array(Rust::groestl_512(data, size));
}

Data Hash::hmac256(const Data& key, const Data& message) {
    return Rust::data_from_c_byte_array(Rust::hmac__sha256(key.data(), key.size(), message.data(), message.size()));
}
