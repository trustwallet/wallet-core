// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWGroestlcoinAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../Groestlcoin/Address.h"

#include <cstring>

bool TWGroestlcoinAddressEqual(struct TWGroestlcoinAddress* _Nonnull lhs, struct TWGroestlcoinAddress* _Nonnull rhs) {
    return lhs->impl.bytes == rhs->impl.bytes;
}

bool TWGroestlcoinAddressIsValidString(TWString* _Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    return TW::Groestlcoin::Address::isValid(s);
}

struct TWGroestlcoinAddress* _Nullable TWGroestlcoinAddressCreateWithString(TWString* _Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    if (!TW::Groestlcoin::Address::isValid(s)) {
        return nullptr;
    }
    return new TWGroestlcoinAddress{TW::Groestlcoin::Address(s)};
}

struct TWGroestlcoinAddress* _Nonnull TWGroestlcoinAddressCreateWithPublicKey(struct TWPublicKey* _Nonnull publicKey, uint8_t prefix) {
    return new TWGroestlcoinAddress{TW::Groestlcoin::Address(publicKey->impl, prefix)};
}

void TWGroestlcoinAddressDelete(struct TWGroestlcoinAddress* _Nonnull address) {
    delete address;
}

TWString* _Nonnull TWGroestlcoinAddressDescription(struct TWGroestlcoinAddress* _Nonnull address) {
    const auto str = address->impl.string();
    return TWStringCreateWithUTF8Bytes(str.c_str());
}
