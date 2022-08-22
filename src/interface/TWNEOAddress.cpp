// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../NEO/Address.h"

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWNEOAddress.h>

using namespace TW;
using namespace TW::NEO;

// TODO: Adjust implementations below

bool TWNEOAddressEqual(struct TWNEOAddress *_Nonnull lhs, struct TWNEOAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWNEOAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return NEO::Address::isValid(*s);
}

struct TWNEOAddress *_Nullable TWNEOAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    try {
        const auto address = NEO::Address(*s);
        return new TWNEOAddress{ std::move(address) };
    } catch (...) {
        return nullptr;
    }
}

struct TWNEOAddress *_Nonnull TWNEOAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWNEOAddress{ NEO::Address(publicKey->impl) };
}

void TWNEOAddressDelete(struct TWNEOAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWNEOAddressDescription(struct TWNEOAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}
