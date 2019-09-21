// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWNEARAddress.h>

#include "../Data.h"
#include "../NEAR/Address.h"

#include <TrustWalletCore/TWPublicKey.h>

#include <memory>
#include <string>
#include <vector>

using namespace TW;
using namespace TW::NEAR;

bool TWNEARAddressEqual(struct TWNEARAddress *_Nonnull lhs, struct TWNEARAddress *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWNEARAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string *>(string);
    return Address::isValid(*s);
}

struct TWNEARAddress *_Nullable TWNEARAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);

    try {
        return new TWNEARAddress{ Address(*s) };
    } catch (...) {
        return nullptr;
    }
}

struct TWNEARAddress *_Nonnull TWNEARAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWNEARAddress{ Address(publicKey->impl) };
}

void TWNEARAddressDelete(struct TWNEARAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWNEARAddressDescription(struct TWNEARAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}
