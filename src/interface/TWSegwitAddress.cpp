// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Bitcoin/SegwitAddress.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWSegwitAddress.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWPublicKey.h>

using namespace TW;

bool TWSegwitAddressEqual(struct TWSegwitAddress *_Nonnull lhs, struct TWSegwitAddress *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWSegwitAddressIsValidString(TWString *_Nonnull string) {
    auto* s = reinterpret_cast<const std::string*>(string);
    return Bitcoin::SegwitAddress::isValid(*s);
}

struct TWSegwitAddress *_Nullable TWSegwitAddressCreateWithString(TWString *_Nonnull string) {
    auto* s = reinterpret_cast<const std::string*>(string);
    auto dec = Bitcoin::SegwitAddress::decode(*s);
    if (!std::get<2>(dec)) {
        return nullptr;
    }

    return new TWSegwitAddress{ std::move(std::get<0>(dec)) };
}

struct TWSegwitAddress *_Nonnull TWSegwitAddressCreateWithPublicKey(enum TWHRP hrp, struct TWPublicKey *_Nonnull publicKey) {
    const auto address = Bitcoin::SegwitAddress(publicKey->impl, stringForHRP(hrp));
    return new TWSegwitAddress{ std::move(address) };
}

void TWSegwitAddressDelete(struct TWSegwitAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWSegwitAddressDescription(struct TWSegwitAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

enum TWHRP TWSegwitAddressHRP(struct TWSegwitAddress *_Nonnull address) {
    return hrpForString(address->impl.hrp.c_str());
}

int TWSegwitAddressWitnessVersion(struct TWSegwitAddress *_Nonnull address) {
    return address->impl.witnessVersion;
}

TWData *_Nonnull TWSegwitAddressWitnessProgram(struct TWSegwitAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.witnessProgram.data(), address->impl.witnessProgram.size());
}
