// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHash.h>

#include <TrezorCrypto/blake2b.h>
#include <TrezorCrypto/ripemd160.h>
#include <TrezorCrypto/sha2.h>
#include <TrezorCrypto/sha3.h>

#include <string.h>


TWData *_Nonnull TWHashSHA1(TWData *_Nonnull data) {
    uint8_t resultBytes[TWHashSHA1Length];
    uint8_t *dataBytes = TWDataBytes(data);
    sha1_Raw(dataBytes, TWDataSize(data), resultBytes);
    return TWDataCreateWithBytes(resultBytes, TWHashSHA1Length);
}

TWData *_Nonnull TWHashSHA256(TWData *_Nonnull data) {
    uint8_t resultBytes[TWHashSHA256Length];
    uint8_t *dataBytes = TWDataBytes(data);
    sha256_Raw(dataBytes, TWDataSize(data), resultBytes);
    return TWDataCreateWithBytes(resultBytes, TWHashSHA256Length);
}

TWData *_Nonnull TWHashSHA512(TWData *_Nonnull data) {
    uint8_t resultBytes[TWHashSHA512Length];
    uint8_t *dataBytes = TWDataBytes(data);
    sha512_Raw(dataBytes, TWDataSize(data), resultBytes);
    return TWDataCreateWithBytes(resultBytes, TWHashSHA512Length);
}

TWData *_Nonnull TWHashKeccak256(TWData *_Nonnull data) {
    uint8_t resultBytes[TWHashSHA256Length];
    uint8_t *dataBytes = TWDataBytes(data);
    keccak_256(dataBytes, TWDataSize(data), resultBytes);
    return TWDataCreateWithBytes(resultBytes, TWHashSHA256Length);
}

TWData *_Nonnull TWHashKeccak512(TWData *_Nonnull data) {
    uint8_t resultBytes[TWHashSHA512Length];
    uint8_t *dataBytes = TWDataBytes(data);
    keccak_512(dataBytes, TWDataSize(data), resultBytes);
    return TWDataCreateWithBytes(resultBytes, TWHashSHA512Length);
}

TWData *_Nonnull TWHashSHA3_256(TWData *_Nonnull data) {
    uint8_t resultBytes[TWHashSHA256Length];
    uint8_t *dataBytes = TWDataBytes(data);
    sha3_256(dataBytes, TWDataSize(data), resultBytes);
    return TWDataCreateWithBytes(resultBytes, TWHashSHA256Length);
}

TWData *_Nonnull TWHashSHA3_512(TWData *_Nonnull data) {
    uint8_t resultBytes[TWHashSHA512Length];
    uint8_t *dataBytes = TWDataBytes(data);
    sha3_512(dataBytes, TWDataSize(data), resultBytes);
    return TWDataCreateWithBytes(resultBytes, TWHashSHA512Length);
}

TWData *_Nonnull TWHashRIPEMD(TWData *_Nonnull data) {
    uint8_t resultBytes[TWHashRipemdLength];
    uint8_t *dataBytes = TWDataBytes(data);
    ripemd160(dataBytes, TWDataSize(data), resultBytes);
    return TWDataCreateWithBytes(resultBytes, TWHashRipemdLength);
}

TWData *_Nonnull TWHashBlake2b(TWData *_Nonnull data, size_t outlen) {
    uint8_t *resultBytes = (uint8_t *) malloc(outlen);
    uint8_t *dataBytes = TWDataBytes(data);
    blake2b(dataBytes, TWDataSize(data), resultBytes, outlen);
    TWData *result = TWDataCreateWithBytes(resultBytes, outlen);
    free(resultBytes);
    return result;
}

TWData *_Nonnull TWHashSHA256RIPEMD(TWData *_Nonnull data) {
    uint8_t round1[TWHashSHA256Length];
    uint8_t *dataBytes = TWDataBytes(data);
    sha256_Raw(dataBytes, TWDataSize(data), round1);

    uint8_t resultBytes[TWHashRipemdLength];
    ripemd160(round1, TWHashSHA256Length, resultBytes);
    return TWDataCreateWithBytes(resultBytes, TWHashRipemdLength);
}

TWData *_Nonnull TWHashSHA256SHA256(TWData *_Nonnull data) {
    uint8_t round1[TWHashSHA256Length];
    uint8_t *dataBytes = TWDataBytes(data);
    sha256_Raw(dataBytes, TWDataSize(data), round1);

    uint8_t round2[TWHashSHA256Length];
    sha256_Raw(round1, TWHashSHA256Length, round2);
    return TWDataCreateWithBytes(round2, TWHashSHA256Length);
}
