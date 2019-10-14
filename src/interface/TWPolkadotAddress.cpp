// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Polkadot/Address.h"
#include "../Data.h"

#include <TrustWalletCore/TWPolkadotAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

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
    if (!Address::isValid(*s)) {
        return nullptr;
    }
    return new TWPolkadotAddress{ Address(*s) };
}

struct TWPolkadotAddress *_Nonnull TWPolkadotAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWPolkadotAddress {Address(publicKey->impl, TWPolkadotAddressTypePolkaDotLiveSS58, 2)};
}

void TWPolkadotAddressDelete(struct TWPolkadotAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWPolkadotAddressDescription(struct TWPolkadotAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}
