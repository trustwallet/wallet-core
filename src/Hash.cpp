// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Hash.h"
#include "XXHash64.h"
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

uint64_t Hash::xxhash(const byte* data, size_t size, uint64_t seed)
{
    return XXHash64::hash(data, size, seed);
}

Data Hash::xxhash64(const byte* data, size_t size, uint64_t seed)
{
    const auto hash = XXHash64::hash(data, size, seed);
    Data result;
    encode64LE(hash, result);
    return result; 
}

Data Hash::xxhash64concat(const byte* data, size_t size)
{
    auto key1 = xxhash64(data, size, 0);
    const auto key2 = xxhash64(data, size, 1);
    TW::append(key1, key2);
    return key1;
}

Data Hash::hmac256(const Data& key, const Data& message) {
    Data hmac(SHA256_DIGEST_LENGTH);
    hmac_sha256(key.data(), static_cast<uint32_t>(key.size()), message.data(), static_cast<uint32_t>(message.size()), hmac.data());
    return hmac;
}
