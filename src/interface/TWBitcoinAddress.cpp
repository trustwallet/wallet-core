// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Base58.h"
#include "../Bitcoin/Address.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWBitcoinAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <cstring>

bool TWBitcoinAddressEqual(struct TWBitcoinAddress *_Nonnull lhs, struct TWBitcoinAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWBitcoinAddressIsValid(TWData *_Nonnull data) {
    return TWDataSize(data) == TW::Bitcoin::Address::size;
}

bool TWBitcoinAddressIsValidString(TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    return TW::Bitcoin::Address::isValid(s);
}

struct TWBitcoinAddress *_Nullable TWBitcoinAddressCreateWithString(TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    try {
        return new TWBitcoinAddress{ TW::Bitcoin::Address(s) };
    } catch (...) {
        return nullptr;
    }
}

struct TWBitcoinAddress *_Nullable TWBitcoinAddressCreateWithData(TWData *_Nonnull data) {
    const auto& d = *reinterpret_cast<const TW::Data*>(data);
    try {
        return new TWBitcoinAddress{ TW::Bitcoin::Address(d) };
    } catch (...) {
        return nullptr;
    }
}

struct TWBitcoinAddress *_Nullable TWBitcoinAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, uint8_t prefix) {
    try {
        return new TWBitcoinAddress{ TW::Bitcoin::Address(publicKey->impl, prefix) };
    } catch (...) {
        return nullptr;
    }
}

void TWBitcoinAddressDelete(struct TWBitcoinAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWBitcoinAddressDescription(struct TWBitcoinAddress *_Nonnull address) {
    return TWStringCreateWithUTF8Bytes(address->impl.string().c_str());
}

uint8_t TWBitcoinAddressPrefix(struct TWBitcoinAddress *_Nonnull address) {
    return address->impl.bytes[0];
}

TWData *_Nonnull TWBitcoinAddressKeyhash(struct TWBitcoinAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data() + 1, TW::Bitcoin::Address::size - 1);
}
