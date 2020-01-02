// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Polkadot/Address.h"

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWPolkadotAddress.h>
#include  <TrustWalletCore/TWSS58AddressType.h>

using namespace TW;
using namespace TW::Polkadot;

bool TWPolkadotAddressEqual(struct TWPolkadotAddress *_Nonnull lhs, struct TWPolkadotAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWPolkadotAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWPolkadotAddress *_Nullable TWPolkadotAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    try {
        const auto address = Address(*s);
        return new TWPolkadotAddress{ std::move(address) };
    } catch (...) {
        return nullptr;
    }
}

struct TWPolkadotAddress *_Nonnull TWPolkadotAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWPolkadotAddress{ Address(publicKey->impl) };
}

void TWPolkadotAddressDelete(struct TWPolkadotAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWPolkadotAddressDescription(struct TWPolkadotAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWPolkadotAddressPublicKey(struct TWPolkadotAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data() + 1, address->impl.bytes.size() - 1);
}
