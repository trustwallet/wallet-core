// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TrustWalletCore/TWData.h"
#include "../Base58.h"
#include "../Nervos/Address.h"

#include <TrustWalletCore/TWNervosAddress.h>

#include <cstring>

bool TWNervosAddressEqual(struct TWNervosAddress *_Nonnull lhs, struct TWNervosAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWNervosAddressIsValidString(TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    return TW::Nervos::Address::isValid(s);
}

struct TWNervosAddress *_Nullable TWNervosAddressCreateWithString(TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    try {
        return new TWNervosAddress{ TW::Nervos::Address(s) };
    } catch (...) {
        return nullptr;
    }
}

void TWNervosAddressDelete(struct TWNervosAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWNervosAddressDescription(struct TWNervosAddress *_Nonnull address) {
    return TWStringCreateWithUTF8Bytes(address->impl.string().c_str());
}

TWData *_Nonnull TWNervosAddressCodeHash(struct TWNervosAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.codeHash.data(), address->impl.codeHash.size());
}

TWString *_Nonnull TWNervosAddressHashType(struct TWNervosAddress *_Nonnull address) {
    return TWStringCreateWithUTF8Bytes(address->impl.hashTypeString().c_str());
}

TWData *_Nonnull TWNervosAddressArgs(struct TWNervosAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.args.data(), address->impl.args.size());
}
