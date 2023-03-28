// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "Data.h"
#include "Coin.h"
#include "CoinEntry.h"
#include "AnyAddress.h"

bool TWAnyAddressEqual(struct TWAnyAddress* _Nonnull lhs, struct TWAnyAddress* _Nonnull rhs) {
    return *lhs->impl == *rhs->impl;
}

bool TWAnyAddressIsValid(TWString* _Nonnull string, enum TWCoinType coin) {
    const auto& address = *reinterpret_cast<const std::string*>(string);
    return TW::validateAddress(coin, address);
}

bool TWAnyAddressIsValidSS58([[maybe_unused]] TWString* string, [[maybe_unused]] enum TWCoinType coin, [[maybe_unused]] uint32_t ss58Prefix) {
    const auto& address = *reinterpret_cast<const std::string*>(string);
    return TW::validateAddress(coin, address, ss58Prefix);
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
    auto *impl = TW::AnyAddress::createAddress(address, coin, hrpStr.c_str());
    if (impl == nullptr) {
        return nullptr;
    }
    return new TWAnyAddress{impl};
}


struct TWAnyAddress* TWAnyAddressCreateSS58(TWString* _Nonnull string, enum TWCoinType coin, uint32_t ss58Prefix) {
    const auto& address = *reinterpret_cast<const std::string*>(string);
    auto *impl = TW::AnyAddress::createAddress(address, coin, ss58Prefix);
    if (impl == nullptr) {
        return nullptr;
    }
    return new TWAnyAddress{impl};
}

struct TWAnyAddress* _Nonnull TWAnyAddressCreateWithPublicKey(
    struct TWPublicKey* _Nonnull publicKey, enum TWCoinType coin) {
    return new TWAnyAddress{TW::AnyAddress::createAddress(publicKey->impl, coin)};
}

struct TWAnyAddress* _Nonnull TWAnyAddressCreateWithPublicKeyDerivation(
    struct TWPublicKey* _Nonnull publicKey, enum TWCoinType coin, enum TWDerivation derivation) {
    return new TWAnyAddress{TW::AnyAddress::createAddress(publicKey->impl, coin, derivation)};
}

struct TWAnyAddress* _Nonnull TWAnyAddressCreateBech32WithPublicKey(
    struct TWPublicKey* _Nonnull publicKey, enum TWCoinType coin, TWString* _Nonnull hrp) {
    const auto& hrpStr = *reinterpret_cast<const std::string*>(hrp);
    return new TWAnyAddress{TW::AnyAddress::createAddress(publicKey->impl, coin, TWDerivationDefault, TW::Bech32Prefix(hrpStr.c_str()))};
}

struct TWAnyAddress* TWAnyAddressCreateSS58WithPublicKey(struct TWPublicKey* publicKey, enum TWCoinType coin, uint32_t ss58Prefix) {
    return new TWAnyAddress{TW::AnyAddress::createAddress(publicKey->impl, coin, TWDerivationDefault, TW::SS58Prefix(ss58Prefix))};
}

struct TWAnyAddress* TWAnyAddressCreateWithPublicKeyFilecoinAddressType(struct TWPublicKey* _Nonnull publicKey, enum TWFilecoinAddressType filecoinAddressType) {
    TW::PrefixVariant prefix = std::monostate();
    if (filecoinAddressType == TWFilecoinAddressTypeDelegated) {
        prefix = TW::DelegatedPrefix();
    }
    return new TWAnyAddress{TW::AnyAddress::createAddress(publicKey->impl, TWCoinTypeFilecoin, TWDerivationDefault, prefix)};
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
