// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Stellar/Address.h"

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWStellarAddress.h>

using namespace TW;
using namespace TW::Stellar;

bool TWStellarAddressEqual(struct TWStellarAddress *_Nonnull lhs, struct TWStellarAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWStellarAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWStellarAddress *_Nullable TWStellarAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    try {
        const auto address = Address(*s);
        return new TWStellarAddress{ std::move(address) };
    } catch (...) {
        return nullptr;
    }
}

struct TWStellarAddress *_Nonnull TWStellarAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWStellarAddress{ Address(publicKey->impl) };
}

void TWStellarAddressDelete(struct TWStellarAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWStellarAddressDescription(struct TWStellarAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWStellarAddressKeyHash(struct TWStellarAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), address->impl.bytes.size());
}
