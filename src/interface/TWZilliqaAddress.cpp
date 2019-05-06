// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.


#include "../Zilliqa/Address.h"
#include "../Data.h"

#include <TrustWalletCore/TWZilliqaAddress.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <memory>
#include <string>

using namespace TW;
using namespace TW::Zilliqa;

bool TWZilliqaAddressEqual(struct TWZilliqaAddress *_Nonnull lhs, struct TWZilliqaAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWZilliqaAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWZilliqaAddress *_Nullable TWZilliqaAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    if (!Address::isValid(*s)) {
        return nullptr;
    }
    return new TWZilliqaAddress{ Address(*s) };
}

struct TWZilliqaAddress *_Nonnull TWZilliqaAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWZilliqaAddress{ Address(publicKey->impl) };
}

void TWZilliqaAddressDelete(struct TWZilliqaAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWZilliqaAddressDescription(struct TWZilliqaAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}
