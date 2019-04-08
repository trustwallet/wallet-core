// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWTronAddress.h>

#include "../Tron/Address.h"

#include <TrustWalletCore/TWPublicKey.h>

using namespace TW;
using namespace TW::Tron;

bool TWTronAddressEqual(struct TWTronAddress* _Nonnull lhs, struct TWTronAddress* _Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWTronAddressIsValidString(TWString* _Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWTronAddress* _Nullable TWTronAddressCreateWithString(TWString* _Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    try {
        const auto address = Address(*s);
        return new TWTronAddress{std::move(address)};
    } catch (std::exception& e) {
        return nullptr;
    }
}

struct TWTronAddress* _Nonnull TWTronAddressCreateWithPublicKey(
    struct TWPublicKey* _Nonnull publicKey) {
    return new TWTronAddress{Address(publicKey->impl)};
}

void TWTronAddressDelete(struct TWTronAddress* _Nonnull address) {
    delete address;
}

TWString* _Nonnull TWTronAddressDescription(struct TWTronAddress* _Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}
