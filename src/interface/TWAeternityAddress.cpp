// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Aeternity/Address.h"
#include "../Data.h"

#include <TrustWalletCore/TWAeternityAddress.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <memory>
#include <string>

using namespace TW;
using namespace TW::Aeternity;

/// Compares two addresses for equality.
bool TWAeternityAddressEqual(struct TWAeternityAddress *_Nonnull lhs, struct TWAeternityAddress *_Nonnull rhs){
    return lhs->impl == rhs->impl;
}

/// Determines if the string is a valid address.
bool TWAeternityAddressIsValidString(TWString *_Nonnull string){
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

/// Creates an address from a string representAeternity.
struct TWAeternityAddress *_Nullable TWAeternityAddressCreateWithString(TWString *_Nonnull string){
    auto s = reinterpret_cast<const std::string*>(string);
    if (!Address::isValid(*s)) {
        return nullptr;
    }
    return new TWAeternityAddress{ Address(*s) };
}

/// Creates an address from a public key.
struct TWAeternityAddress *_Nonnull TWAeternityAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey){
    return new TWAeternityAddress{ Address(publicKey->impl) };
}

/// Returns the address string representation.
TWString *_Nonnull TWAeternityAddressDescription(struct TWAeternityAddress *_Nonnull address){
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

void TWAeternityAddressDelete(struct TWAeternityAddress *_Nonnull address) {
    delete address;
}
