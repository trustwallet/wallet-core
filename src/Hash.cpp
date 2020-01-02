// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Hash.h"

#include <TrezorCrypto/blake256.h>
#include <TrezorCrypto/blake2b.h>
#include <TrezorCrypto/groestl.h>
#include <TrezorCrypto/ripemd160.h>
#include <TrezorCrypto/sha2.h>
#include <TrezorCrypto/sha3.h>

#include <string>

using namespace TW;

Data Hash::sha1(const byte* begin, const byte* end) {
    Data result(sha1Size);
    sha1_Raw(begin, end - begin, result.data());
    return result;
}

Data Hash::sha256(const byte* begin, const byte* end) {
    Data result(sha256Size);
    sha256_Raw(begin, end - begin, result.data());
    return result;
}

Data Hash::sha512(const byte* begin, const byte* end) {
    Data result(sha512Size);
    sha512_Raw(begin, end - begin, result.data());
    return result;
}

Data Hash::sha512_256(const byte* begin, const byte* end) {
    Data result(sha256Size);
    sha512_256_Raw(begin, end - begin, result.data());
    return result;
}

Data Hash::keccak256(const byte* begin, const byte* end) {
    Data result(sha256Size);
    keccak_256(begin, end - begin, result.data());
    return result;
}

Data Hash::keccak512(const byte* begin, const byte* end) {
    Data result(sha512Size);
    keccak_512(begin, end - begin, result.data());
    return result;
}

Data Hash::sha3_256(const byte* begin, const byte* end) {
    Data result(sha256Size);
    ::sha3_256(begin, end - begin, result.data());
    return result;
}

Data Hash::sha3_512(const byte* begin, const byte* end) {
    Data result(sha512Size);
    ::sha3_512(begin, end - begin, result.data());
    return result;
}

Data Hash::ripemd(const byte* begin, const byte* end) {
    Data result(ripemdSize);
    ::ripemd160(begin, static_cast<uint32_t>(end - begin), result.data());
    return result;
}

Data Hash::blake256(const byte* begin, const byte* end) {
    Data result(sha256Size);
    ::blake256(begin, end - begin, result.data());
    return result;
}

Data Hash::blake2b(const byte* begin, const byte* end, size_t size) {
    Data result(size);
    ::blake2b(begin, static_cast<uint32_t>(end - begin), result.data(), size);
    return result;
}

Data Hash::blake2b(const byte* begin, const byte* end, size_t size, const Data& personal) {
    Data result(size);
    ::blake2b_Personal(begin, static_cast<uint32_t>(end - begin), personal.data(), personal.size(), result.data(), size);
    return result;
}

Data Hash::groestl512(const byte* begin, const byte* end) {
    GROESTL512_CTX ctx;
    Data result(sha512Size);
    groestl512_Init(&ctx);
    groestl512_Update(&ctx, begin, end - begin);
    groestl512_Final(&ctx, result.data());
    return result;
}
