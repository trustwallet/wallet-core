// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Cosmos/Address.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWCosmosAddress.h>

using namespace TW;
using namespace TW::Cosmos;

bool TWCosmosAddressEqual(struct TWCosmosAddress *_Nonnull lhs, struct TWCosmosAddress *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWCosmosAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWCosmosAddress *_Nullable TWCosmosAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    Address address;
    if (!Address::decode(*s, address)) {
        return nullptr;
    }

    return new TWCosmosAddress{ address };
}

struct TWCosmosAddress *_Nullable TWCosmosAddressCreateWithKeyHash(enum TWHRP hrp, TWData *_Nonnull keyHash) {
    auto d = reinterpret_cast<const std::vector<uint8_t>*>(keyHash);
    return new TWCosmosAddress{ Address(stringForHRP(hrp), *d) };
}

struct TWCosmosAddress *_Nonnull TWCosmosAddressCreateWithPublicKey(enum TWHRP hrp, struct TWPublicKey *_Nonnull publicKey) {
    return new TWCosmosAddress{ Address(stringForHRP(hrp), publicKey->impl) };
}

void TWCosmosAddressDelete(struct TWCosmosAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWCosmosAddressDescription(struct TWCosmosAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

enum TWHRP TWCosmosAddressHRP(struct TWCosmosAddress *_Nonnull address) {
    return hrpForString(address->impl.getHrp().c_str());
}

TWData *_Nonnull TWCosmosAddressKeyHash(struct TWCosmosAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.getKeyHash().data(), address->impl.getKeyHash().size());
}
