// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWFIOAddress.h>

#include "../Data.h"
#include "../FIO/Address.h"

#include <TrustWalletCore/TWPublicKey.h>

#include <memory>
#include <string>
#include <vector>

using namespace TW;
using namespace TW::FIO;

bool TWFIOAddressEqual(struct TWFIOAddress *_Nonnull lhs, struct TWFIOAddress *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWFIOAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string *>(string);
    return Address::isValid(*s);
}

struct TWFIOAddress *_Nullable TWFIOAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);

    try {
        return new TWFIOAddress{ Address(*s) };
    } catch (...) {
        return nullptr;
    }
}

struct TWFIOAddress *_Nonnull TWFIOAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWFIOAddress{ Address(publicKey->impl) };
}

void TWFIOAddressDelete(struct TWFIOAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWFIOAddressDescription(struct TWFIOAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}
