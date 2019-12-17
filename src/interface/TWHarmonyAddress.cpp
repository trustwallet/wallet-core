// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHarmonyAddress.h>

#include "../Data.h"
#include "../Harmony/Address.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrezorCrypto/ecdsa.h>
#include <memory>
#include <string>

using namespace TW;
using namespace TW::Harmony;

bool TWHarmonyAddressEqual(struct TWHarmonyAddress *_Nonnull lhs,
                           struct TWHarmonyAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWHarmonyAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string *>(string);
    return Address::isValid(*s);
}

struct TWHarmonyAddress *_Nullable TWHarmonyAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string *>(string);
    Address address;
    if (!Address::decode(*s, address)) {
        return nullptr;
    }
    return new TWHarmonyAddress{address};
}

struct TWHarmonyAddress *_Nullable TWHarmonyAddressCreateWithKeyHash(TWData *_Nonnull keyHash) {
    auto d = reinterpret_cast<const Data *>(keyHash);
    auto address = Address(*d);
    if (address.getKeyHash().size() == 0) {
        return nullptr;
    }
    return new TWHarmonyAddress{address};
}

struct TWHarmonyAddress *_Nonnull TWHarmonyAddressCreateWithPublicKey(
    struct TWPublicKey *_Nonnull publicKey) {
    return new TWHarmonyAddress{Address(publicKey->impl)};
}

void TWHarmonyAddressDelete(struct TWHarmonyAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWHarmonyAddressDescription(struct TWHarmonyAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWHarmonyAddressKeyHash(struct TWHarmonyAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.getKeyHash().data(), address->impl.getKeyHash().size());
}
