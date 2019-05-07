// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWSemuxAddress.h>

#include "../Semux/Address.h"

#include <memory>

using namespace TW::Semux;

bool TWSemuxAddressEqual(struct TWSemuxAddress *_Nonnull lhs, struct TWSemuxAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWSemuxAddressIsValid(TWData *_Nonnull data) {
    return TWDataSize(data) == Address::size;
}

bool TWSemuxAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWSemuxAddress *_Nullable TWSemuxAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    try {
        const auto address = Address(*s);
        return new TWSemuxAddress{ std::move(address) };
    } catch (...) {
        return nullptr;
    }
}

struct TWSemuxAddress *_Nonnull TWSemuxAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWSemuxAddress{ Address(publicKey->impl) };
}

void TWSemuxAddressDelete(struct TWSemuxAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWSemuxAddressDescription(struct TWSemuxAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

