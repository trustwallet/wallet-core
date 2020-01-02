// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWOntologyAddress.h>

#include "../Data.h"
#include "../Ontology/Address.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrezorCrypto/ecdsa.h>
#include <memory>
#include <string>

using namespace TW;
using namespace TW::Ontology;

bool TWOntologyAddressEqual(struct TWOntologyAddress *_Nonnull lhs, struct TWOntologyAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWOntologyAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string *>(string);
    return Address::isValid(*s);
}

struct TWOntologyAddress *_Nullable TWOntologyAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string *>(string);
    if (!Address::isValid(*s)) {
        return nullptr;
    }
    return new TWOntologyAddress{Address(*s)};
}

struct TWOntologyAddress *_Nonnull TWOntologyAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWOntologyAddress{Address(publicKey->impl)};
}

void TWOntologyAddressDelete(struct TWOntologyAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWOntologyAddressDescription(struct TWOntologyAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWOntologyAddressKeyHash(struct TWOntologyAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.data.data(), address->impl.data.size());
}
