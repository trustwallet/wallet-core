// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWIoTeXAddress.h>

#include "../Data.h"
#include "../IoTeX/Address.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrezorCrypto/ecdsa.h>
#include <memory>
#include <string>

using namespace TW;
using namespace TW::IoTeX;

bool TWIoTeXAddressEqual(struct TWIoTeXAddress *_Nonnull lhs, struct TWIoTeXAddress *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWIoTeXAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWIoTeXAddress *_Nullable TWIoTeXAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    if (!Address::isValid(*s)) {
        return nullptr;
    }
    return new TWIoTeXAddress{ Address(*s) };
}

struct TWIoTeXAddress *_Nullable TWIoTeXAddressCreateWithKeyHash(TWData *_Nonnull keyHash) {
    auto d = reinterpret_cast<const Data*>(keyHash);
    if (!Address::isValid(*d)) {
        return nullptr;
    }
    return new TWIoTeXAddress{ Address(*d) };
}

struct TWIoTeXAddress *_Nonnull TWIoTeXAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWIoTeXAddress{ Address(publicKey->impl) };
}

void TWIoTeXAddressDelete(struct TWIoTeXAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWIoTeXAddressDescription(struct TWIoTeXAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWIoTeXAddressKeyHash(struct TWIoTeXAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.keyHash.data(), address->impl.keyHash.size());
}
