// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWPublicKey.h>


#include "../Coin.h"
#include "../Address.h"

using namespace TW;

bool TWAnyAddressEqual(struct TWAnyAddress* _Nonnull lhs, struct TWAnyAddress* _Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWAnyAddressIsValid(TWString* _Nonnull string, enum TWCoinType coin) {
    const auto& address = *reinterpret_cast<const std::string*>(string);
    return TW::validateAddress(coin, address);
}

struct TWAnyAddress* _Nullable TWAnyAddressCreateWithString(TWString* _Nonnull string,
                                                            enum TWCoinType coin) {
    const auto& address = *reinterpret_cast<const std::string*>(string);
    try {
        return new TWAnyAddress{Address(address, coin)};
    } catch (...) {
        return nullptr;
    }
}

struct TWAnyAddress* _Nonnull TWAnyAddressCreateWithPublicKey(
    struct TWPublicKey* _Nonnull publicKey, enum TWCoinType coin) {
    return new TWAnyAddress{Address(publicKey, coin)};
}

void TWAnyAddressDelete(struct TWAnyAddress* _Nonnull address) {
    delete address;
}

TWString* _Nonnull TWAnyAddressDescription(struct TWAnyAddress* _Nonnull address) {
    return TWStringCreateWithUTF8Bytes(address->impl.address.c_str());
}

enum TWCoinType TWAnyAddressCoin(struct TWAnyAddress* _Nonnull address) {
    return address->impl.coin;
}

TWData* _Nonnull TWAnyAddressData(struct TWAnyAddress* _Nonnull address) {
    return address->impl.getData();
}
