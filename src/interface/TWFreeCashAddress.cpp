// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Base58.h"
#include "../FreeCash/Address.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWFreeCashAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <cstring>

using namespace TW::FreeCash;

bool TWFreeCashAddressEqual(struct TWFreeCashAddress *_Nonnull lhs, struct TWFreeCashAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWFreeCashAddressIsValid(TWData *_Nonnull data) {
    return TWDataSize(data) == Address::size;
}

bool TWFreeCashAddressIsValidString(TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    return Address::isValid(s);
}

struct TWFreeCashAddress *_Nullable TWFreeCashAddressCreateWithString(TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    try {
        return new TWFreeCashAddress{ Address(s) };
    } catch (...) {
        return nullptr;
    }
}

struct TWFreeCashAddress *_Nullable TWFreeCashAddressCreateWithData(TWData *_Nonnull data) {
    auto& d = *reinterpret_cast<const TW::Data*>(data);
    try {
        return new TWFreeCashAddress{ Address(d) };
    } catch (...) {
        return nullptr;
    }
}

struct TWFreeCashAddress *_Nullable TWFreeCashAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, uint8_t prefix) {
    try {
        return new TWFreeCashAddress{ Address(publicKey->impl, prefix) };
    } catch (...) {
        return nullptr;
    }
}

void TWFreeCashAddressDelete(struct TWFreeCashAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWFreeCashAddressDescription(struct TWFreeCashAddress *_Nonnull address) {
    return TWStringCreateWithUTF8Bytes(address->impl.string().c_str());
}

uint8_t TWFreeCashAddressPrefix(struct TWFreeCashAddress *_Nonnull address) {
    return address->impl.bytes[0];
}

TWData *_Nonnull TWFreeCashAddressKeyhash(struct TWFreeCashAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data() + 1, Address::size - 1);
}
