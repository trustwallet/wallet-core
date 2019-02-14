// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBase58.h>

#include <TrezorCrypto/base58.h>
#include <string>

TWString *_Nonnull TWBase58Encode(TWData *_Nonnull data) {
    size_t size = 0;
    b58enc(nullptr, &size, TWDataBytes(data), TWDataSize(data));
    size += 16;
    
    auto string = std::string();
    string.resize(size);
    base58_encode_check(TWDataBytes(data), (int)TWDataSize(data), HASHER_SHA2D, &string[0], (int)size);

    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWString *_Nonnull TWBase58EncodeNoCheck(TWData *_Nonnull data) {
    size_t size = 0;
    b58enc(nullptr, &size, TWDataBytes(data), TWDataSize(data));

    auto string = std::string();
    string.resize(size);
    b58enc(&string[0], &size, TWDataBytes(data), TWDataSize(data));

    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nullable TWBase58Decode(TWString *_Nonnull string) {
    size_t capacity = 128;
    uint8_t result[capacity];

    int size = base58_decode_check(TWStringUTF8Bytes(string), HASHER_SHA2D, result, (int)capacity);
    if (size == 0) {
        return nullptr;
    }

    return TWDataCreateWithBytes(result, size);
}

TWData *_Nullable TWBase58DecodeNoCheck(TWString *_Nonnull string) {
    size_t capacity = 128;
    size_t size = capacity;
    uint8_t result[capacity];
    if (!b58tobin(result, &size, TWStringUTF8Bytes(string))) {
        return nullptr;
    }

    return TWDataCreateWithBytes(result + capacity - size, size);
}
