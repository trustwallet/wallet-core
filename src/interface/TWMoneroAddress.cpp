// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWMoneroAddress.h>

#include "../Monero/Address.h"

#include <memory>

using namespace TW::Monero;

bool TWMoneroAddressEqual(struct TWMoneroAddress *_Nonnull lhs, struct TWMoneroAddress *_Nonnull rhs){
    return lhs->impl == rhs->impl;
}

bool TWMoneroAddressIsValid(TWData *_Nonnull data){
    return TWDataSize(data) == Address::size;
}

bool TWMoneroAddressIsValidString(TWString *_Nonnull string){
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWMoneroAddress *_Nonnull TWMoneroAddressCreateWithString(TWString *_Nonnull string){
    auto s = reinterpret_cast<const std::string*>(string);
    try {
        const auto address = Address(*s);
        return new TWMoneroAddress{ std::move(address) };
    } catch (...){
        return nullptr;
    }
}

struct TWMoneroAddress *_Nonnull TWMoneroAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey){
    return new TWMoneroAddress{ Address( 1, publicKey->impl, publicKey->impl)};
    //    return new TWMoneroAddress{ Address(publicKey->impl)};

}

void TWMoneroAddressDelete(struct TWMoneroAddress *_Nonnull address){
    delete address;
}

TWString *_Nonnull TWMoneroAddressDescription(struct TWMoneroAddress *_Nonnull address){
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}