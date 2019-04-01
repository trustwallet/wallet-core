// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Data.h"

#include <TrustWalletCore/TWHash.h>
#include <TrezorCrypto/blake2b.h>
#include <TrezorCrypto/ripemd160.h>
#include <TrezorCrypto/sha2.h>
#include <TrezorCrypto/sha3.h>

#include <array>

TWData *_Nonnull TWHashSHA1(TWData *_Nonnull data) {
    std::array<uint8_t, TWHashSHA1Length> resultBytes;
    auto dataBytes = TWDataBytes(data);
    sha1_Raw(dataBytes, TWDataSize(data), resultBytes.data());
    return TWDataCreateWithBytes(resultBytes.data(), TWHashSHA1Length);
}

TWData *_Nonnull TWHashSHA256(TWData *_Nonnull data) {
    std::array<uint8_t, TWHashSHA256Length> resultBytes;
    auto dataBytes = TWDataBytes(data);
    sha256_Raw(dataBytes, TWDataSize(data), resultBytes.data());
    return TWDataCreateWithBytes(resultBytes.data(), TWHashSHA256Length);
}

TWData *_Nonnull TWHashSHA512(TWData *_Nonnull data) {
    std::array<uint8_t, TWHashSHA512Length> resultBytes;
    auto dataBytes = TWDataBytes(data);
    sha512_Raw(dataBytes, TWDataSize(data), resultBytes.data());
    return TWDataCreateWithBytes(resultBytes.data(), TWHashSHA512Length);
}

TWData *_Nonnull TWHashKeccak256(TWData *_Nonnull data) {
    std::array<uint8_t, TWHashSHA256Length> resultBytes;
    auto dataBytes = TWDataBytes(data);
    keccak_256(dataBytes, TWDataSize(data), resultBytes.data());
    return TWDataCreateWithBytes(resultBytes.data(), TWHashSHA256Length);
}

TWData *_Nonnull TWHashKeccak512(TWData *_Nonnull data) {
    std::array<uint8_t, TWHashSHA512Length> resultBytes;
    auto dataBytes = TWDataBytes(data);
    keccak_512(dataBytes, TWDataSize(data), resultBytes.data());
    return TWDataCreateWithBytes(resultBytes.data(), TWHashSHA512Length);
}

TWData *_Nonnull TWHashSHA3_256(TWData *_Nonnull data) {
    std::array<uint8_t, TWHashSHA256Length> resultBytes;
    auto dataBytes = TWDataBytes(data);
    sha3_256(dataBytes, TWDataSize(data), resultBytes.data());
    return TWDataCreateWithBytes(resultBytes.data(), TWHashSHA256Length);
}

TWData *_Nonnull TWHashSHA3_512(TWData *_Nonnull data) {
    std::array<uint8_t, TWHashSHA512Length> resultBytes;
    auto dataBytes = TWDataBytes(data);
    sha3_512(dataBytes, TWDataSize(data), resultBytes.data());
    return TWDataCreateWithBytes(resultBytes.data(), TWHashSHA512Length);
}

TWData *_Nonnull TWHashRIPEMD(TWData *_Nonnull data) {
    std::array<uint8_t, TWHashRipemdLength> resultBytes;
    auto dataBytes = TWDataBytes(data);
    ripemd160(dataBytes, TWDataSize(data), resultBytes.data());
    return TWDataCreateWithBytes(resultBytes.data(), TWHashRipemdLength);
}

TWData *_Nonnull TWHashBlake2b(TWData *_Nonnull data, size_t outlen) {
    auto resultBytes = TW::Data(outlen);
    auto dataBytes = TWDataBytes(data);
    blake2b(dataBytes, TWDataSize(data), resultBytes.data(), outlen);
    auto result = TWDataCreateWithBytes(resultBytes.data(), outlen);
    return result;
}

TWData *_Nonnull TWHashSHA256RIPEMD(TWData *_Nonnull data) {
    std::array<uint8_t, TWHashSHA256Length> round1;
    auto dataBytes = TWDataBytes(data);
    sha256_Raw(dataBytes, TWDataSize(data), round1.data());

    std::array<uint8_t, TWHashRipemdLength> resultBytes;
    ripemd160(round1.data(), TWHashSHA256Length, resultBytes.data());
    return TWDataCreateWithBytes(resultBytes.data(), TWHashRipemdLength);
}

TWData *_Nonnull TWHashSHA256SHA256(TWData *_Nonnull data) {
    std::array<uint8_t, TWHashSHA256Length> round1;
    auto dataBytes = TWDataBytes(data);
    sha256_Raw(dataBytes, TWDataSize(data), round1.data());

    std::array<uint8_t, TWHashSHA256Length> round2;
    sha256_Raw(round1.data(), TWHashSHA256Length, round2.data());
    return TWDataCreateWithBytes(round2.data(), TWHashSHA256Length);
}
