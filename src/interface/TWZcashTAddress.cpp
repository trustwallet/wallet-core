// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Base58.h"
#include "../PublicKey.h"
#include "../Zcash/TAddress.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWZcashTAddress.h>

#include <cstring>
#include <string>
#include <vector>

using namespace TW;

bool TWZcashTAddressEqual(struct TWZcashTAddress *_Nonnull lhs, struct TWZcashTAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWZcashTAddressIsValid(TWData *_Nonnull data) {
    return TWDataSize(data) == TWZcashTAddressSize;
}

bool TWZcashTAddressIsValidString(TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    return Zcash::TAddress::isValid(s);
}

struct TWZcashTAddress *_Nullable TWZcashTAddressCreateWithString(TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    try {
        return new TWZcashTAddress{ Zcash::TAddress(s) };
    } catch (...) {
        return nullptr;
    }
}

struct TWZcashTAddress *_Nullable TWZcashTAddressCreateWithData(TWData *_Nonnull data) {
    auto& d = *reinterpret_cast<const TW::Data*>(data);
    try {
        return new TWZcashTAddress{ Zcash::TAddress(d) };
    } catch (...) {
        return nullptr;
    }
}

struct TWZcashTAddress *_Nullable TWZcashTAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, uint8_t prefix) {
    try {
        return new TWZcashTAddress{ Zcash::TAddress(publicKey->impl, prefix) };
    } catch (...) {
        return nullptr;
    }
}

void TWZcashTAddressDelete(struct TWZcashTAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWZcashTAddressDescription(struct TWZcashTAddress *_Nonnull address) {
    return TWStringCreateWithUTF8Bytes(address->impl.string().c_str());
}
