// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.


#include "../Aion/Address.h"
#include "../Data.h"

#include <TrustWalletCore/TWAionAddress.h>
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
