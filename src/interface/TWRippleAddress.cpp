// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWRippleAddress.h>

#include "../Ripple/Address.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/ripple/base58.h>
#include <string.h>
#include <memory>

using namespace TW;
using namespace TW::Ripple;

bool TWRippleAddressEqual(struct TWRippleAddress *_Nonnull lhs, struct TWRippleAddress *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWRippleAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWRippleAddress *_Nullable TWRippleAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    const auto address = Address(*s);
    return new TWRippleAddress{ std::move(address) };
}

struct TWRippleAddress *_Nullable TWRippleAddressCreateWithData(TWData *_Nonnull data) {
    auto d = reinterpret_cast<const std::vector<uint8_t>*>(data);
    return new TWRippleAddress{ Address(*d) };
}

struct TWRippleAddress *_Nonnull TWRippleAddressCreateWithPublicKey(struct TWPublicKey publicKey) {
    std::vector<uint8_t> data;
    if (TWPublicKeyIsCompressed(publicKey)) {
        data.insert(data.end(), publicKey.bytes, publicKey.bytes + PublicKey::compressedSize);
    } else {
        data.insert(data.end(), publicKey.bytes, publicKey.bytes + PublicKey::uncompressedSize);
    }
    const auto address = Address(PublicKey(data));
    return new TWRippleAddress{ std::move(address) };
}

void TWRippleAddressDelete(struct TWRippleAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWRippleAddressDescription(struct TWRippleAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWRippleAddressKeyHash(struct TWRippleAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), Address::size);
}
