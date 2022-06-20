// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWPublicKey.h>


#include "../Coin.h"

using namespace TW;

struct TWAnyAddress {
    TWString* address;
    enum TWCoinType coin;
};

bool TWAnyAddressEqual(struct TWAnyAddress* _Nonnull lhs, struct TWAnyAddress* _Nonnull rhs) {
    return TWStringEqual(lhs->address, rhs->address) && lhs->coin == rhs->coin;
}

bool TWAnyAddressIsValid(TWString* _Nonnull string, enum TWCoinType coin) {
    const auto& address = *reinterpret_cast<const std::string*>(string);
    return TW::validateAddress(coin, address);
}

struct TWAnyAddress* _Nullable TWAnyAddressCreateWithString(TWString* _Nonnull string,
                                                            enum TWCoinType coin) {
    const auto& address = *reinterpret_cast<const std::string*>(string);
    auto normalized = TW::normalizeAddress(coin, address);
    if (normalized.empty()) { return nullptr; }
    return new TWAnyAddress{TWStringCreateWithUTF8Bytes(normalized.c_str()), coin};
}

struct TWAnyAddress* _Nonnull TWAnyAddressCreateWithPublicKey(
    struct TWPublicKey* _Nonnull publicKey, enum TWCoinType coin) {
    auto address = TW::deriveAddress(coin, publicKey->impl);
    return new TWAnyAddress{TWStringCreateWithUTF8Bytes(address.c_str()), coin};
}

void TWAnyAddressDelete(struct TWAnyAddress* _Nonnull address) {
    TWStringDelete(address->address);
    delete address;
}

TWString* _Nonnull TWAnyAddressDescription(struct TWAnyAddress* _Nonnull address) {
    return TWStringCreateWithUTF8Bytes(TWStringUTF8Bytes(address->address));
}

enum TWCoinType TWAnyAddressCoin(struct TWAnyAddress* _Nonnull address) {
    return address->coin;
}

TWData* _Nonnull TWAnyAddressData(struct TWAnyAddress* _Nonnull address) {
    const auto& string = *reinterpret_cast<const std::string*>(address->address);
    Data data;
    try {
        data = TW::addressToData(address->coin, string);
    } catch (...) {}
    return TWDataCreateWithBytes(data.data(), data.size());
}
