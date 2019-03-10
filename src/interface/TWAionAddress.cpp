// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAionAddress.h>

#include "../Data.h"
#include "../Aion/Address.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <memory>
#include <string>

using namespace TW;
using namespace TW::Aion;

bool TWAionAddressEqual(struct TWAionAddress *_Nonnull lhs, struct TWAionAddress *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWAionAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWAionAddress *_Nullable TWAionAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    if (!Address::isValid(*s)) {
        return nullptr;
    }
    return new TWAionAddress{ Address(*s) };
}

struct TWAionAddress *_Nullable TWAionAddressCreateWithData(TWData *_Nonnull data) {
    auto d = reinterpret_cast<const std::vector<uint8_t>*>(data);
    return new TWAionAddress{ Address(*d) };
}

struct TWAionAddress *_Nonnull TWAionAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWAionAddress{ Address(publicKey->impl) };
}

void TWAionAddressDelete(struct TWAionAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWAionAddressDescription(struct TWAionAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWAionAddressData(struct TWAionAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), address->impl.bytes.size());
}
