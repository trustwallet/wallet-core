// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Hash.h"
#include "BinaryCoding.h"

#include "rust/bindgen/WalletCoreRSBindgen.h"
#include <TrezorCrypto/blake256.h>
#include <TrezorCrypto/blake2b.h>
#include <TrezorCrypto/groestl.h>
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
    auto raw_res = Rust::sha1(data, size);
    if (raw_res.data == nullptr || raw_res.size == 0) {
        return Data(sha1Size);
    }
    Data result(&raw_res.data[0], &raw_res.data[raw_res.size]);
    std::free(raw_res.data);
    return result;
}

Data Hash::sha256(const byte* data, size_t size) {
    auto raw_res = Rust::sha256(data, size);
    if (raw_res.data == nullptr || raw_res.size == 0) {
        return Data(sha256Size);
    }
    Data result(&raw_res.data[0], &raw_res.data[raw_res.size]);
    std::free(raw_res.data);
    return result;
}

Data Hash::sha512(const byte* data, size_t size) {
    auto raw_res = Rust::sha512(data, size);
    if (raw_res.data == nullptr || raw_res.size == 0) {
        return Data(sha512Size);
    }
    Data result(&raw_res.data[0], &raw_res.data[raw_res.size]);
    std::free(raw_res.data);
    return result;
}

Data Hash::sha512_256(const byte* data, size_t size) {
    auto raw_res = Rust::sha512_256(data, size);
    if (raw_res.data == nullptr || raw_res.size == 0) {
        return Data(sha256Size);
    }
    Data result(&raw_res.data[0], &raw_res.data[raw_res.size]);
    std::free(raw_res.data);
    return result;
}

Data Hash::keccak256(const byte* data, size_t size) {
    auto raw_res = Rust::keccak256(data, size);
    if (raw_res.data == nullptr || raw_res.size == 0) {
        return Data(sha256Size);
    }
    Data result(&raw_res.data[0], &raw_res.data[raw_res.size]);
    std::free(raw_res.data);
    return result;
}

Data Hash::keccak512(const byte* data, size_t size) {
    auto raw_res = Rust::keccak512(data, size);
    if (raw_res.data == nullptr || raw_res.size == 0) {
        return Data(sha512Size);
    }
    Data result(&raw_res.data[0], &raw_res.data[raw_res.size]);
    std::free(raw_res.data);
    return result;
}

Data Hash::sha3_256(const byte* data, size_t size) {
    auto raw_res = Rust::sha3__256(data, size);
    if (raw_res.data == nullptr || raw_res.size == 0) {
        return Data(sha256Size);
    }
    Data result(&raw_res.data[0], &raw_res.data[raw_res.size]);
    std::free(raw_res.data);
    return result;
}

Data Hash::sha3_512(const byte* data, size_t size) {
    auto raw_res = Rust::sha3__512(data, size);
    if (raw_res.data == nullptr || raw_res.size == 0) {
        return Data(sha512Size);
    }
    Data result(&raw_res.data[0], &raw_res.data[raw_res.size]);
    std::free(raw_res.data);
    return result;
}

Data Hash::ripemd(const byte* data, size_t size) {
    auto raw_res = Rust::ripemd_160(data, size);
    if (raw_res.data == nullptr || raw_res.size == 0) {
        return Data(ripemdSize);
    }
    Data result(&raw_res.data[0], &raw_res.data[raw_res.size]);
    std::free(raw_res.data);
    return result;
}

Data Hash::blake256(const byte* data, size_t size) {
    Data result(sha256Size);
    ::blake256(data, size, result.data());
    return result;
}

Data Hash::blake2b(const byte* data, size_t dataSize, size_t hashSize) {
    auto raw_res = Rust::blake2_b(data, dataSize, hashSize);
    if (raw_res.data == nullptr || raw_res.size == 0) {
        return Data(hashSize);
    }
    Data result(&raw_res.data[0], &raw_res.data[raw_res.size]);
    std::free(raw_res.data);
    return result;
}

Data Hash::blake2b(const byte* data, size_t dataSize, size_t hashSize, const Data& personal) {
    auto raw_res = Rust::blake2_b_personal(data, dataSize, hashSize, personal.data(), personal.size());
    if (raw_res.data == nullptr || raw_res.size == 0) {
        return Data(hashSize);
    }
    Data result(&raw_res.data[0], &raw_res.data[raw_res.size]);
    std::free(raw_res.data);
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
