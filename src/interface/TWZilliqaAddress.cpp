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
    return Zilliqa::Address::isValid(*s);
}

struct TWZilliqaAddress *_Nullable TWZilliqaAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    Address address;
    if (!Address::decode(*s, address)) {
        return nullptr;
    }

    return new TWZilliqaAddress{ address };
}

struct TWZilliqaAddress *_Nullable TWZilliqaAddressCreateWithKeyHash(TWData *_Nonnull keyHash) {
    auto data = reinterpret_cast<const std::vector<uint8_t>*>(keyHash);
    return new TWZilliqaAddress{ Address(*data) };
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

TWString *_Nonnull TWZilliqaAddressKeyHash(struct TWZilliqaAddress *_Nonnull address) {
    const auto string = Zilliqa::checkSum(address->impl.getKeyHash());
    return TWStringCreateWithUTF8Bytes(string.c_str());
 }
