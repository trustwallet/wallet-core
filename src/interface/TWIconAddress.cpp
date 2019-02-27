// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWIconAddress.h>

#include "../Data.h"
#include "../Icon/Address.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrezorCrypto/ecdsa.h>
#include <memory>
#include <string>

using namespace TW;
using namespace TW::Icon;

bool TWIconAddressEqual(struct TWIconAddress *_Nonnull lhs, struct TWIconAddress *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWIconAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWIconAddress *_Nullable TWIconAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    if (!Address::isValid(*s)) {
        return nullptr;
    }
    return new TWIconAddress{ Address(*s) };
}

struct TWIconAddress *_Nullable TWIconAddressCreateWithKeyHash(TWData *_Nonnull keyHash, TWIconAddressType type) {
    auto d = reinterpret_cast<const Data*>(keyHash);
    if (!Address::isValid(*d)) {
        return nullptr;
    }
    return new TWIconAddress{ Address(*d, type) };
}

struct TWIconAddress *_Nonnull TWIconAddressCreateWithPublicKey(struct TWPublicKey publicKey, TWIconAddressType type) {
    Data data;
    if (TWPublicKeyIsCompressed(publicKey)) {
        data.insert(data.end(), publicKey.bytes, publicKey.bytes + PublicKey::compressedSize);
    } else {
        data.insert(data.end(), publicKey.bytes, publicKey.bytes + PublicKey::uncompressedSize);
    }
    return new TWIconAddress{ Address(PublicKey(data), type) };
}

void TWIconAddressDelete(struct TWIconAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWIconAddressDescription(struct TWIconAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWIconAddressKeyHash(struct TWIconAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), address->impl.bytes.size());
}
