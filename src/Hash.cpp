// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Hash.h"
#include "BinaryCoding.h"

#include <TrezorCrypto/blake256.h>
#include <TrezorCrypto/blake2b.h>
#include <TrezorCrypto/groestl.h>
#include <TrezorCrypto/ripemd160.h>
#include <TrezorCrypto/sha2.h>
#include <TrezorCrypto/sha3.h>
#include <TrezorCrypto/hmac.h>

#include <string>

using namespace TW;

TW::Hash::HasherSimpleType Hash::functionPointerFromEnum(TW::Hash::Hasher hasher) {
    switch (hasher) {
        case Hash::HasherSha1: return Hash::sha1;
        default: case Hash::HasherSha256: return Hash::sha256;
        case Hash::HasherSha512: return Hash::sha512;
        case Hash::HasherSha512_256: return Hash::sha512_256;
        case Hash::HasherKeccak256: return Hash::keccak256;
        case Hash::HasherKeccak512: return Hash::keccak512;
        case Hash::HasherSha3_256: return Hash::sha3_256;
        case Hash::HasherSha3_512: return Hash::sha3_512;
        case Hash::HasherRipemd: return Hash::ripemd;
        case Hash::HasherBlake256: return Hash::blake256;
        case Hash::HasherGroestl512: return Hash::groestl512;
        case Hash::HasherSha256d: return Hash::sha256d;
        case Hash::HasherSha256ripemd: return Hash::sha256ripemd;
        case Hash::HasherSha3_256ripemd: return Hash::sha3_256ripemd;
        case Hash::HasherBlake256d: return Hash::blake256d;
        case Hash::HasherBlake256ripemd: return Hash::blake256ripemd;
        case Hash::HasherGroestl512d: return Hash::groestl512d;
    }
}

Data Hash::sha1(const byte* data, size_t size) {
    Data result(sha1Size);
    sha1_Raw(data, size, result.data());
    return result;
}

Data Hash::sha256(const byte* data, size_t size) {
    Data result(sha256Size);
    sha256_Raw(data, size, result.data());
    return result;
}

Data Hash::sha512(const byte* data, size_t size) {
    Data result(sha512Size);
    sha512_Raw(data, size, result.data());
    return result;
}

Data Hash::sha512_256(const byte* data, size_t size) {
    Data result(sha256Size);
    sha512_256_Raw(data, size, result.data());
    return result;
}

Data Hash::keccak256(const byte* data, size_t size) {
    Data result(sha256Size);
    keccak_256(data, size, result.data());
    return result;
}

Data Hash::keccak512(const byte* data, size_t size) {
    Data result(sha512Size);
    keccak_512(data, size, result.data());
    return result;
}

Data Hash::sha3_256(const byte* data, size_t size) {
    Data result(sha256Size);
    ::sha3_256(data, size, result.data());
    return result;
}

Data Hash::sha3_512(const byte* data, size_t size) {
    Data result(sha512Size);
    ::sha3_512(data, size, result.data());
    return result;
}

Data Hash::ripemd(const byte* data, size_t size) {
    Data result(ripemdSize);
    ::ripemd160(data, static_cast<uint32_t>(size), result.data());
    return result;
}

Data Hash::blake256(const byte* data, size_t size) {
    Data result(sha256Size);
    ::blake256(data, size, result.data());
    return result;
}

Data Hash::blake2b(const byte* data, size_t dataSize, size_t hashSize) {
    Data result(hashSize);
    ::blake2b(data, static_cast<uint32_t>(dataSize), result.data(), hashSize);
    return result;
}

Data Hash::blake2b(const byte* data, size_t dataSize, size_t hashSize, const Data& personal) {
    Data result(hashSize);
    ::blake2b_Personal(data, static_cast<uint32_t>(dataSize), personal.data(), personal.size(), result.data(), hashSize);
    return result;
}

Data Hash::groestl512(const byte* data, size_t size) {
    GROESTL512_CTX ctx;
    Data result(sha512Size);
    groestl512_Init(&ctx);
    groestl512_Update(&ctx, data, size);
    groestl512_Final(&ctx, result.data());
    return result;
}

Data Hash::hmac256(const Data& key, const Data& message) {
    Data hmac(SHA256_DIGEST_LENGTH);
    hmac_sha256(key.data(), static_cast<uint32_t>(key.size()), message.data(), static_cast<uint32_t>(message.size()), hmac.data());
    return hmac;
}
