// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../TON/Address.h"

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWTONAddress.h>

using namespace TW;
using namespace TW::TON;

bool TWTONAddressEqual(struct TWTONAddress *_Nonnull lhs, struct TWTONAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWTONAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWTONAddress *_Nullable TWTONAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    try {
        const auto address = Address(*s);
        return new TWTONAddress{ std::move(address) };
    } catch (...) {
        return nullptr;
    }
}

struct TWTONAddress *_Nonnull TWTONAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWTONAddress{ Address(publicKey->impl) };
}

void TWTONAddressDelete(struct TWTONAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWTONAddressDescription(struct TWTONAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}
