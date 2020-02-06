// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Cardano/AddressV3.h"

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWCardanoAddress.h>

using namespace TW;
using namespace TW::Cardano;

bool TWCardanoAddressEqual(struct TWCardanoAddress *_Nonnull lhs, struct TWCardanoAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWCardanoAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return AddressV3::isValid(*s);
}

struct TWCardanoAddress *_Nullable TWCardanoAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    try {
        const auto address = AddressV3(*s);
        return new TWCardanoAddress{ std::move(address) };
    } catch (...) {
        return nullptr;
    }
}

struct TWCardanoAddress *_Nonnull TWCardanoAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWCardanoAddress{ AddressV3(publicKey->impl) };
}

void TWCardanoAddressDelete(struct TWCardanoAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWCardanoAddressDescription(struct TWCardanoAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}
