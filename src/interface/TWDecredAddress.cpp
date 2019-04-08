// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWDecredAddress.h>

#include "../Data.h"
#include "../Decred/Address.h"

using namespace TW;
using namespace TW::Decred;

bool TWDecredAddressEqual(struct TWDecredAddress *_Nonnull lhs, struct TWDecredAddress *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWDecredAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWDecredAddress *_Nullable TWDecredAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    if (!Address::isValid(*s)) {
        return nullptr;
    }
    return new TWDecredAddress{ Address(*s) };
}

struct TWDecredAddress *_Nonnull TWDecredAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWDecredAddress{ Address(publicKey->impl) };
}

void TWDecredAddressDelete(struct TWDecredAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWDecredAddressDescription(struct TWDecredAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}
