// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBech32Address.h>

#include "../Bitcoin/Bech32Address.h"
#include "../Bech32.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWHRP.h>

#include <TrezorCrypto/ecdsa.h>
#include <string.h>
#include <memory>

using namespace TW;
using namespace TW::Bitcoin;

bool TWBech32AddressEqual(struct TWBech32Address *_Nonnull lhs, struct TWBech32Address *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWBech32AddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Bech32Address::isValid(*s);
}

struct TWBech32Address *_Nullable TWBech32AddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    auto dec = Bech32Address::decode(*s);
    if (!dec.second) {
        return nullptr;
    }

    return new TWBech32Address{ std::move(dec.first) };
}

struct TWBech32Address *_Nullable TWBech32AddressCreateWithData(enum TWHRP hrp, TWData *_Nonnull data) {
    auto d = reinterpret_cast<const std::vector<uint8_t>*>(data);
    return new TWBech32Address{ Bech32Address(stringForHRP(hrp), 0, *d) };
}

struct TWBech32Address *_Nonnull TWBech32AddressCreateWithPublicKey(enum TWHRP hrp, struct TWPublicKey publicKey) {
    std::vector<uint8_t> data;
    if (TWPublicKeyIsCompressed(publicKey)) {
        data.insert(data.end(), publicKey.bytes, publicKey.bytes + PublicKey::compressedSize);
    } else {
        data.insert(data.end(), publicKey.bytes, publicKey.bytes + PublicKey::uncompressedSize);
    }
    const auto address = Bech32Address(PublicKey(data), 0, stringForHRP(hrp));
    return new TWBech32Address{ std::move(address) };
}

void TWBech32AddressDelete(struct TWBech32Address *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWBech32AddressDescription(struct TWBech32Address *_Nonnull address) {
    const auto string = address->impl.encode();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

enum TWHRP TWBech32AddressHRP(struct TWBech32Address *_Nonnull address) {
    return hrpForString(address->impl.hrp.c_str());
}

TWData *_Nonnull TWBech32AddressWitnessProgram(struct TWBech32Address *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.witnessProgram.data(), address->impl.witnessProgram.size());
}
