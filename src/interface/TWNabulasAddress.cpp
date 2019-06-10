// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWNebulasAddress.h>

#include "../Data.h"
#include "../Nebulas/Address.h"

#include <TrustWalletCore/TWPublicKey.h>

#include <memory>
#include <string>
#include <vector>

using namespace TW;
using namespace TW::Nebulas;


bool TWNebulasAddressEqual(struct TWNebulasAddress *_Nonnull lhs, struct TWNebulasAddress *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWNebulasAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string *>(string);
    return Address::isValid(*s);
}

struct TWNebulasAddress *_Nullable TWNebulasAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);

    try {
        return new TWNebulasAddress{ Address(*s) };
    } catch (...) {
        return nullptr;
    }
}

struct TWNebulasAddress *_Nonnull TWNebulasAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, uint8_t prefix) {
    const NormalType = 87;
    const ContractType = 88;
    return new TWNebulasAddress{ Address(publicKey->impl, prefix) };
}

struct TWNebulasAddress *_Nullable TWNebulasAddressCreateWithKeyHash(TWData *_Nonnull keyHash, enum TWNebulasKeyType type) {
    auto d = reinterpret_cast<const Data *>(keyHash);
    try {
        return new TWNebulasAddress{ Address(*d, translateType(type)) };
    } catch (...) {
        return nullptr;
    }
}

void TWNebulasAddressDelete(struct TWNebulasAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWNebulasAddressDescription(struct TWNebulasAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}