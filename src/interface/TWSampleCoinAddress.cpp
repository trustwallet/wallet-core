// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../SampleCoin/Address.h"

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWSampleCoinAddress.h>

using namespace TW;
using namespace TW::SampleCoin;

// TODO: Adjust implementations below

bool TWSampleCoinAddressEqual(struct TWSampleCoinAddress *_Nonnull lhs, struct TWSampleCoinAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWSampleCoinAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWSampleCoinAddress *_Nullable TWSampleCoinAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    try {
        const auto address = Address(*s);
        return new TWSampleCoinAddress{ std::move(address) };
    } catch (...) {
        return nullptr;
    }
}

struct TWSampleCoinAddress *_Nonnull TWSampleCoinAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWSampleCoinAddress{ Address(publicKey->impl) };
}

void TWSampleCoinAddressDelete(struct TWSampleCoinAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWSampleCoinAddressDescription(struct TWSampleCoinAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWSampleCoinAddressKeyHash(struct TWSampleCoinAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.keyHash().data(), address->impl.keyHash().size());
}
