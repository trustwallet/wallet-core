// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWTendermintAddress.h>

#include "../Tendermint/Address.h"
#include "../Bech32.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWHRP.h>

#include <TrezorCrypto/ecdsa.h>
#include <string.h>
#include <memory>

using namespace TW;
using namespace TW::Tendermint;

bool TWTendermintAddressEqual(struct TWTendermintAddress *_Nonnull lhs, struct TWTendermintAddress *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWTendermintAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWTendermintAddress *_Nullable TWTendermintAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    auto dec = Address::decode(*s);
    if (!dec.second) {
        return nullptr;
    }

    return new TWTendermintAddress{ std::move(dec.first) };
}

struct TWTendermintAddress *_Nullable TWTendermintAddressCreateWithKeyHash(enum TWHRP hrp, TWData *_Nonnull keyHash) {
    auto d = reinterpret_cast<const std::vector<uint8_t>*>(keyHash);
    return new TWTendermintAddress{ Address(stringForHRP(hrp), *d) };
}

struct TWTendermintAddress *_Nonnull TWTendermintAddressCreateWithPublicKey(enum TWHRP hrp, struct TWPublicKey *_Nonnull publicKey) {
    return new TWTendermintAddress{ Address(stringForHRP(hrp), publicKey->impl) };
}

void TWTendermintAddressDelete(struct TWTendermintAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWTendermintAddressDescription(struct TWTendermintAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

enum TWHRP TWTendermintAddressHRP(struct TWTendermintAddress *_Nonnull address) {
    return hrpForString(address->impl.hrp.c_str());
}

TWData *_Nonnull TWTendermintAddressKeyHash(struct TWTendermintAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.keyHash.data(), address->impl.keyHash.size());
}
