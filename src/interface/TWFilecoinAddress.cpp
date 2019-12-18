// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWFilecoinAddress.h>

#include "../Filecoin/Address.h"

#include <memory>

using namespace TW::Filecoin;

bool TWFilecoinAddressEqual(struct TWFilecoinAddress *_Nonnull lhs, struct TWFilecoinAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWFilecoinAddressIsValid(TWData *_Nonnull data) {
    std::vector<uint8_t> buf;
    auto ptr = TWDataBytes(data);
    buf.assign(ptr, ptr + TWDataSize(data));
    return Address::isValid(buf);
}

bool TWFilecoinAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWFilecoinAddress *_Nullable TWFilecoinAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    try {
        const auto address = Address(*s);
        return new TWFilecoinAddress{ std::move(address) };
    } catch (...) {
        return nullptr;
    }
}

struct TWFilecoinAddress *_Nonnull TWFilecoinAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWFilecoinAddress{ Address(publicKey->impl) };
}

void TWFilecoinAddressDelete(struct TWFilecoinAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWFilecoinAddressDescription(struct TWFilecoinAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWFilecoinAddressKeyHash(struct TWFilecoinAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), address->impl.bytes.size());
}
