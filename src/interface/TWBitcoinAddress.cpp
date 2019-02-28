// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBitcoinAddress.h>

#include "../Bitcoin/Address.h"

#include <TrustWalletCore/TWPublicKey.h>
#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/ecdsa.h>

using namespace TW::Bitcoin;

bool TWBitcoinAddressEqual(struct TWBitcoinAddress lhs, struct TWBitcoinAddress rhs) {
    return memcmp(lhs.bytes, rhs.bytes, Address::size) == 0;
}

bool TWBitcoinAddressIsValid(TWData *_Nonnull data) {
    return TWDataSize(data) == Address::size;
}

bool TWBitcoinAddressIsValidString(TWString *_Nonnull string) {
    auto str = TWStringUTF8Bytes(string);

    size_t capacity = 128;
    uint8_t buffer[capacity];

    int size = base58_decode_check(str, HASHER_SHA2D, buffer, (int)capacity);
    if (size != Address::size) {
        return false;
    }

    return true;
}

bool TWBitcoinAddressInitWithString(struct TWBitcoinAddress *_Nonnull address, TWString *_Nonnull string) {
    auto str = TWStringUTF8Bytes(string);

    size_t capacity = 128;
    uint8_t buffer[capacity];

    int size = base58_decode_check(str, HASHER_SHA2D, buffer, (int)capacity);
    if (size != Address::size) {
        return false;
    }

    memcpy(address->bytes, buffer, Address::size);
    return true;
}

bool TWBitcoinAddressInitWithData(struct TWBitcoinAddress *_Nonnull address, TWData *_Nonnull data) {
    if (TWDataSize(data) != Address::size) {
        return false;
    }
    TWDataCopyBytes(data, 0, Address::size, address->bytes);
    return true;
}

void TWBitcoinAddressInitWithPublicKey(struct TWBitcoinAddress *_Nonnull address, struct TWPublicKey publicKey, uint8_t prefix) {
    address->bytes[0] = prefix;

    auto compressed = TWPublicKeyCompressed(publicKey);
    ecdsa_get_pubkeyhash(compressed.bytes, HASHER_SHA2_RIPEMD,  address->bytes + 1);
}

TWString *_Nonnull TWBitcoinAddressDescription(struct TWBitcoinAddress address) {
    size_t size = 0;
    b58enc(nullptr, &size, address.bytes, Address::size);
    size += 16;

    std::vector<char> str(size);
    base58_encode_check(address.bytes, Address::size, HASHER_SHA2D, str.data(), size);

    return TWStringCreateWithUTF8Bytes(str.data());
}

TWData *_Nonnull TWBitcoinAddressData(struct TWBitcoinAddress address) {
    return TWDataCreateWithBytes(address.bytes, Address::size);
}
