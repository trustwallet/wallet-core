// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Algorand/Address.h"

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWAlgorandAddress.h>

using namespace TW;
using namespace TW::Algorand;

bool TWAlgorandAddressEqual(struct TWAlgorandAddress *_Nonnull lhs, struct TWAlgorandAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWAlgorandAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWAlgorandAddress *_Nullable TWAlgorandAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    try {
        const auto address = Address(*s);
        return new TWAlgorandAddress{ std::move(address) };
    } catch (...) {
        return nullptr;
    }
}

struct TWAlgorandAddress *_Nonnull TWAlgorandAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWAlgorandAddress{ Address(publicKey->impl) };
}

void TWAlgorandAddressDelete(struct TWAlgorandAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWAlgorandAddressDescription(struct TWAlgorandAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWAlgorandAddressKeyHash(struct TWAlgorandAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), address->impl.bytes.size());
}
