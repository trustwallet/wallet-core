// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWIocoinAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../Iocoin/Address.h"

#include <cstring>

using namespace TW::Iocoin;

bool TWIocoinAddressEqual(struct TWIocoinAddress *_Nonnull lhs, struct TWIocoinAddress *_Nonnull rhs) {
    return lhs->impl.bytes == rhs->impl.bytes;
}

bool TWIocoinAddressIsValidString(TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    return Address::isValid(s);
}

struct TWIocoinAddress *_Nullable TWIocoinAddressCreateWithString(TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    if (!Address::isValid(s)) {
        return nullptr;
    }
    return new TWIocoinAddress{ Address(s) };
}

struct TWIocoinAddress *_Nonnull TWIocoinAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, uint8_t prefix) {
    return new TWIocoinAddress{ Address(publicKey->impl, prefix) };
}

void TWIocoinAddressDelete(struct TWIocoinAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWIocoinAddressDescription(struct TWIocoinAddress *_Nonnull address) {
    const auto str = address->impl.string();
    return TWStringCreateWithUTF8Bytes(str.c_str());
}
