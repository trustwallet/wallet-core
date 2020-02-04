// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Bech32.h"
#include "../FreeCash/SegwitAddress.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWFreeCashSegwitAddress.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWPublicKey.h>

using namespace TW;
using namespace TW::FreeCash;

bool TWFreeCashSegwitAddressEqual(struct TWFreeCashSegwitAddress *_Nonnull lhs, struct TWFreeCashSegwitAddress *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWFreeCashSegwitAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return SegwitAddress::isValid(*s);
}

struct TWFreeCashSegwitAddress *_Nullable TWFreeCashSegwitAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    auto dec = SegwitAddress::decode(*s);
    if (!dec.second) {
        return nullptr;
    }

    return new TWFreeCashSegwitAddress{ std::move(dec.first) };
}

struct TWFreeCashSegwitAddress *_Nonnull TWFreeCashSegwitAddressCreateWithPublicKey(enum TWHRP hrp, struct TWPublicKey *_Nonnull publicKey) {
    const auto address = SegwitAddress(publicKey->impl, 0, stringForHRP(hrp));
    return new TWFreeCashSegwitAddress{ std::move(address) };
}

void TWFreeCashSegwitAddressDelete(struct TWFreeCashSegwitAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWFreeCashSegwitAddressDescription(struct TWFreeCashSegwitAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

enum TWHRP TWFreeCashSegwitAddressHRP(struct TWFreeCashSegwitAddress *_Nonnull address) {
    return hrpForString(address->impl.hrp.c_str());
}

TWData *_Nonnull TWFreeCashSegwitAddressWitnessProgram(struct TWFreeCashSegwitAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.witnessProgram.data(), address->impl.witnessProgram.size());
}
