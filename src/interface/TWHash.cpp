// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHash.h>
#include "../Hash.h"
#include "Data.h"

#include "BinaryCoding.h"
#include <TrezorCrypto/blake2b.h>
#include <TrezorCrypto/ripemd160.h>
#include <TrezorCrypto/sha2.h>
#include <TrezorCrypto/sha3.h>

#include <array>

using namespace TW;

TWData* _Nonnull TWHashSHA1(TWData* _Nonnull data) {
    const auto result = Hash::sha1(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data));
    return TWDataCreateWithData(&result);
}

TWData* _Nonnull TWHashSHA256(TWData* _Nonnull data) {
    const auto result = Hash::sha256(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data));
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData* _Nonnull TWHashSHA512(TWData* _Nonnull data) {
    const auto result = Hash::sha512(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data));
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData* _Nonnull TWHashSHA512_256(TWData* _Nonnull data) {
    const auto result = Hash::sha512_256(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data));
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData* _Nonnull TWHashKeccak256(TWData* _Nonnull data) {
    const auto result = Hash::keccak256(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data));
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData* _Nonnull TWHashKeccak512(TWData* _Nonnull data) {
    const auto result = Hash::keccak512(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data));
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData* _Nonnull TWHashSHA3_256(TWData* _Nonnull data) {
    const auto result = Hash::sha3_256(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data));
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData* _Nonnull TWHashSHA3_512(TWData* _Nonnull data) {
    const auto result = Hash::sha3_512(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data));
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData* _Nonnull TWHashRIPEMD(TWData* _Nonnull data) {
    const auto result = Hash::ripemd(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data));
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData* _Nonnull TWHashBlake256(TWData* _Nonnull data) {
    const auto result = Hash::blake256(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data));
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData* _Nonnull TWHashBlake2b(TWData* _Nonnull data, size_t outlen) {
    const auto result = Hash::blake2b(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data), outlen);
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData* _Nonnull TWHashGroestl512(TWData* _Nonnull data) {
    const auto result = Hash::groestl512(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data));
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData* _Nonnull TWHashSHA256SHA256(TWData* _Nonnull data) {
    const auto result = Hash::sha256d(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data));
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData *_Nonnull TWHashBlake2bPersonal(TWData *_Nonnull data, TWData * _Nonnull personal, size_t outlen) {
    auto resultBytes = TW::Data(outlen);
    auto dataBytes = TWDataBytes(data);
    auto personalBytes = TWDataBytes(personal);
    auto personalSize = TWDataSize(personal);
    blake2b_Personal(dataBytes, static_cast<uint32_t>(TWDataSize(data)), personalBytes, personalSize, resultBytes.data(), outlen);
    auto result = TWDataCreateWithBytes(resultBytes.data(), outlen);
    return result;
}

TWData* _Nonnull TWHashSHA256RIPEMD(TWData* _Nonnull data) {
    const auto result = Hash::sha256ripemd(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data));
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData* _Nonnull TWHashSHA3_256RIPEMD(TWData* _Nonnull data) {
    const auto result = Hash::sha3_256ripemd(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data));
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData* _Nonnull TWHashBlake256Blake256(TWData* _Nonnull data) {
    const auto result = Hash::blake256d(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data));
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData* _Nonnull TWHashBlake256RIPEMD(TWData* _Nonnull data) {
    const auto result = Hash::blake256ripemd(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data));
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData* _Nonnull TWHashGroestl512Groestl512(TWData* _Nonnull data) {
    const auto result = Hash::groestl512d(reinterpret_cast<const byte*>(TWDataBytes(data)), TWDataSize(data));
    return TWDataCreateWithBytes(result.data(), result.size());
}