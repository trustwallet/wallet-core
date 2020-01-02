// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWWanchainAddress.h>

#include "../Data.h"
#include "../Wanchain/Address.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrezorCrypto/ecdsa.h>
#include <memory>
#include <string>

using namespace TW;
using namespace TW::Wanchain;

bool TWWanchainAddressEqual(struct TWWanchainAddress *_Nonnull lhs, struct TWWanchainAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWWanchainAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWWanchainAddress *_Nullable TWWanchainAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    if (!Address::isValid(*s)) {
        return nullptr;
    }
    return new TWWanchainAddress{ Address(*s) };
}

struct TWWanchainAddress *_Nullable TWWanchainAddressCreateWithKeyHash(TWData *_Nonnull keyHash) {
    auto d = reinterpret_cast<const Data*>(keyHash);
    if (!Address::isValid(*d)) {
        return nullptr;
    }
    return new TWWanchainAddress{ Address(*d) };
}

struct TWWanchainAddress *_Nonnull TWWanchainAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWWanchainAddress{ Address(publicKey->impl) };
}

void TWWanchainAddressDelete(struct TWWanchainAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWWanchainAddressDescription(struct TWWanchainAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWWanchainAddressKeyHash(struct TWWanchainAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), address->impl.bytes.size());
}
