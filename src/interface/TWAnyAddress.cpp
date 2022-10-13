// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "Data.h"
#include "Coin.h"
#include "AnyAddress.h"

bool TWAnyAddressEqual(struct TWAnyAddress* _Nonnull lhs, struct TWAnyAddress* _Nonnull rhs) {
    return *lhs->impl == *rhs->impl;
}

bool TWAnyAddressIsValid(TWString* _Nonnull string, enum TWCoinType coin) {
    const auto& address = *reinterpret_cast<const std::string*>(string);
    return TW::validateAddress(coin, address);
}

bool TWAnyAddressIsValidBech32(TWString* _Nonnull string, enum TWCoinType coin, TWString* _Nonnull hrp) {
    const auto& address = *reinterpret_cast<const std::string*>(string);
    const auto& hrpStr = *reinterpret_cast<const std::string*>(hrp);
    return TW::validateAddress(coin, address, hrpStr.c_str());
}

struct TWAnyAddress* _Nullable TWAnyAddressCreateWithString(TWString* _Nonnull string,
                                                            enum TWCoinType coin) {
    const auto& address = *reinterpret_cast<const std::string*>(string);
    auto *impl = TW::AnyAddress::createAddress(address, coin);
    if (impl == nullptr) {
        return nullptr;
    }
    return new TWAnyAddress{impl};
}

struct TWAnyAddress* _Nullable TWAnyAddressCreateBech32(TWString* _Nonnull string,
                                                            enum TWCoinType coin, TWString* _Nonnull hrp) {
    const auto& address = *reinterpret_cast<const std::string*>(string);
    const auto& hrpStr = *reinterpret_cast<const std::string*>(hrp);
    auto *impl = TW::AnyAddress::createAddress(address, coin, hrpStr);
    if (impl == nullptr) {
        return nullptr;
    }
    return new TWAnyAddress{impl};
}

struct TWAnyAddress* _Nonnull TWAnyAddressCreateWithPublicKey(
    struct TWPublicKey* _Nonnull publicKey, enum TWCoinType coin) {
    return new TWAnyAddress{TW::AnyAddress::createAddress(publicKey->impl, coin)};
}

struct TWAnyAddress* _Nonnull TWAnyAddressCreateBech32WithPublicKey(
    struct TWPublicKey* _Nonnull publicKey, enum TWCoinType coin, TWString* _Nonnull hrp) {
    const auto& hrpStr = *reinterpret_cast<const std::string*>(hrp);
    return new TWAnyAddress{TW::AnyAddress::createAddress(publicKey->impl, coin, hrpStr)};
}

void TWAnyAddressDelete(struct TWAnyAddress* _Nonnull address) {
    delete address->impl;
    delete address;
}

TWString* _Nonnull TWAnyAddressDescription(struct TWAnyAddress* _Nonnull address) {
    return TWStringCreateWithUTF8Bytes(address->impl->address.c_str());
}

enum TWCoinType TWAnyAddressCoin(struct TWAnyAddress* _Nonnull address) {
    return address->impl->coin;
}

TWData* _Nonnull TWAnyAddressData(struct TWAnyAddress* _Nonnull address) {
    auto data = address->impl->getData();
    return TWDataCreateWithBytes(data.data(), data.size());
}
