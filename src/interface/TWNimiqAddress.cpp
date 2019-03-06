// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWNimiqAddress.h>

#include "../Nimiq/Address.h"

#include <memory>

using namespace TW::Nimiq;

bool TWNimiqAddressEqual(struct TWNimiqAddress *_Nonnull lhs, struct TWNimiqAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWNimiqAddressIsValid(TWData *_Nonnull data) {
    return TWDataSize(data) == Address::size;
}

bool TWNimiqAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWNimiqAddress *_Nullable TWNimiqAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    const auto address = Address(*s);
    return new TWNimiqAddress{ std::move(address) };
}

struct TWNimiqAddress *_Nullable TWNimiqAddressCreateWithData(TWData *_Nonnull data) {
    auto d = reinterpret_cast<const std::vector<uint8_t>*>(data);
    return new TWNimiqAddress{ Address(*d) };
}

struct TWNimiqAddress *_Nonnull TWNimiqAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWNimiqAddress{ Address(publicKey->impl) };
}

void TWNimiqAddressDelete(struct TWNimiqAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWNimiqAddressDescription(struct TWNimiqAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWNimiqAddressKeyHash(struct TWNimiqAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), Address::size);
}
