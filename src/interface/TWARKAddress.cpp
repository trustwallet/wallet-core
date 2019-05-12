// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#include "../ARK/Address.h"

#include <TrustWalletCore/TWARKAddress.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>

using namespace TW;
using namespace TW::ARK;

/// Compares two addresses for equality.
bool TWARKAddressEqual(struct TWARKAddress *_Nonnull lhs, struct TWARKAddress *_Nonnull rhs){
    return lhs->impl == rhs->impl;
}

/// Determines if the string is a valid address.
bool TWARKAddressIsValidString(TWString *_Nonnull string){
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
};

/// Creates an address from a string representaion.
struct TWARKAddress *_Nullable TWARKAddressCreateWithString(TWString *_Nonnull string){
    auto s = reinterpret_cast<const std::string*>(string);
    if (!Address::isValid(*s)) {
        return nullptr;
    }
    return new TWARKAddress{ Address(*s) };
};

/// Creates an address from a public key.
struct TWARKAddress *_Nonnull TWARKAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey){
    return new TWARKAddress{ Address(publicKey->impl) };
};

/// Returns the address string representation.
TWString *_Nonnull TWARKAddressDescription(struct TWARKAddress *_Nonnull address){
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

void TWARKAddressDelete(struct TWARKAddress *_Nonnull address) {
    delete address;
}
