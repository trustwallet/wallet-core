// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWZcashTAddress.h>

#include "../PublicKey.h"

#include <TrustWalletCore/TWPublicKey.h>
#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/ecdsa.h>

#include <cstring>
#include <string>
#include <vector>

using TW::PublicKey;

bool TWZcashTAddressEqual(struct TWZcashTAddress lhs, struct TWZcashTAddress rhs) {
    return std::memcmp(lhs.bytes, rhs.bytes, TWZcashTAddressSize) == 0;
}

bool TWZcashTAddressIsValid(TWData *_Nonnull data) {
    return TWDataSize(data) == TWZcashTAddressSize;
}

bool TWZcashTAddressIsValidString(TWString *_Nonnull string) {
    auto str = TWStringUTF8Bytes(string);

    size_t capacity = 128;
    uint8_t buffer[capacity];

    int size = base58_decode_check(str, HASHER_SHA2D, buffer, (int)capacity);
    if (size != TWZcashTAddressSize) {
        return false;
    }

    return true;
}

bool TWZcashTAddressInitWithString(struct TWZcashTAddress *_Nonnull address, TWString *_Nonnull string) {
    auto str = TWStringUTF8Bytes(string);

    size_t capacity = 128;
    uint8_t buffer[capacity];

    int size = base58_decode_check(str, HASHER_SHA2D, buffer, (int)capacity);
    if (size != TWZcashTAddressSize) {
        return false;
    }

    memcpy(address->bytes, buffer, TWZcashTAddressSize);
    return true;
}

bool TWZcashTAddressInitWithData(struct TWZcashTAddress *_Nonnull address, TWData *_Nonnull data) {
    if (TWDataSize(data) != TWZcashTAddressSize) {
        return false;
    }
    TWDataCopyBytes(data, 0, TWZcashTAddressSize, address->bytes);
    return true;
}

bool TWZcashTAddressInitWithPublicKey(struct TWZcashTAddress *_Nonnull address, struct TWPublicKey *_Nonnull publicKey, uint8_t prefix) {
    // Zcash taddr has two prefix bytes, the first byte is the same 0x1c -> t
    address->bytes[0] = 0x1c;
    address->bytes[1] = prefix;

    auto compressed = publicKey->impl.compressed();
    ecdsa_get_pubkeyhash(compressed.bytes.data(), HASHER_SHA2_RIPEMD,  address->bytes + 2);

    return true;
}

TWString *_Nonnull TWZcashTAddressDescription(struct TWZcashTAddress address) {
    size_t size = 0;
    b58enc(nullptr, &size, address.bytes, TWZcashTAddressSize);
    size += 16;

    std::string str(size, '\0');
    const auto actualSize = base58_encode_check(address.bytes, TWZcashTAddressSize, HASHER_SHA2D, &str[0], size);
    str.erase(str.begin() + actualSize - 1, str.end());

    return TWStringCreateWithUTF8Bytes(str.data());
}

TWData *_Nonnull TWZcashTAddressData(struct TWZcashTAddress address) {
    return TWDataCreateWithBytes(address.bytes, TWZcashTAddressSize);
}
