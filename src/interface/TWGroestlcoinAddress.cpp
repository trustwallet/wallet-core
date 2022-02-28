// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWGroestlcoinAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../Groestlcoin/Address.h"

#include <cstring>

using namespace TW::Groestlcoin;

bool TWGroestlcoinAddressEqual(struct TWGroestlcoinAddress *_Nonnull lhs, struct TWGroestlcoinAddress *_Nonnull rhs) {
    return lhs->impl.bytes == rhs->impl.bytes;
}

bool TWGroestlcoinAddressIsValidString(TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    return Address::isValid(s);
}

struct TWGroestlcoinAddress *_Nullable TWGroestlcoinAddressCreateWithString(TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    if (!Address::isValid(s)) {
        return nullptr;
    }
    return new TWGroestlcoinAddress{ Address(s) };
}

struct TWGroestlcoinAddress *_Nonnull TWGroestlcoinAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, uint8_t prefix) {
    return new TWGroestlcoinAddress{ Address(publicKey->impl, prefix) };
}

void TWGroestlcoinAddressDelete(struct TWGroestlcoinAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWGroestlcoinAddressDescription(struct TWGroestlcoinAddress *_Nonnull address) {
    const auto str = address->impl.string();
    return TWStringCreateWithUTF8Bytes(str.c_str());
}
