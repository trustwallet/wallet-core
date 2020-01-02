// Copyright © 2019 Mart Roosmaa.
// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Nano/Address.h"

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWNanoAddress.h>

using namespace TW;
using namespace TW::Nano;

bool TWNanoAddressEqual(struct TWNanoAddress *_Nonnull lhs, struct TWNanoAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWNanoAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWNanoAddress *_Nullable TWNanoAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    try {
        const auto address = Address(*s);
        return new TWNanoAddress{ std::move(address) };
    } catch (...) {
        return nullptr;
    }
}

struct TWNanoAddress *_Nonnull TWNanoAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWNanoAddress{ Address(publicKey->impl) };
}

void TWNanoAddressDelete(struct TWNanoAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWNanoAddressDescription(struct TWNanoAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWNanoAddressKeyHash(struct TWNanoAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), address->impl.bytes.size());
}
