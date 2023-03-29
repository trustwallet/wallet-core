// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../XRP/XAddress.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWRippleXAddress.h>

#include <memory>

using namespace TW;

bool TWRippleXAddressEqual(struct TWRippleXAddress *_Nonnull lhs, struct TWRippleXAddress *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWRippleXAddressIsValidString(TWString *_Nonnull string) {
    auto* s = reinterpret_cast<const std::string*>(string);
    return Ripple::XAddress::isValid(*s);
}

struct TWRippleXAddress *_Nullable TWRippleXAddressCreateWithString(TWString *_Nonnull string) {
    auto* s = reinterpret_cast<const std::string*>(string);
    try {
        const auto address = Ripple::XAddress(*s);
        return new TWRippleXAddress{ std::move(address) };
    } catch (...) {
        return nullptr;
    }
}

struct TWRippleXAddress *_Nonnull TWRippleXAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, const uint32_t tag) {
    return new TWRippleXAddress{ Ripple::XAddress(publicKey->impl, tag) };
}

void TWRippleXAddressDelete(struct TWRippleXAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWRippleXAddressDescription(struct TWRippleXAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

uint32_t TWRippleXAddressTag(struct TWRippleXAddress *_Nonnull address) {
    return address->impl.tag;
}
