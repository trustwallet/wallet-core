// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBase58.h>

#include "../Base58.h"

#include <string>

using namespace TW;

TWString *_Nonnull TWBase58Encode(TWData *_Nonnull data) {
    const auto& d = *reinterpret_cast<const Data*>(data);
    const auto str = Base58::bitcoin.encodeCheck(d);
    return TWStringCreateWithUTF8Bytes(str.c_str());
}

TWString *_Nonnull TWBase58EncodeNoCheck(TWData *_Nonnull data) {
    auto& d = *reinterpret_cast<const Data*>(data);
    const auto encoded = Base58::bitcoin.encode(d);
    return TWStringCreateWithUTF8Bytes(encoded.c_str());
}

TWData *_Nullable TWBase58Decode(TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    const auto decoded = Base58::bitcoin.decodeCheck(s);
    if (decoded.empty()) {
        return nullptr;
    }

    return TWDataCreateWithBytes(decoded.data(), decoded.size());
}

TWData *_Nullable TWBase58DecodeNoCheck(TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    const auto decoded = Base58::bitcoin.decode(s);
    if (decoded.empty()) {
        return nullptr;
    }

    return TWDataCreateWithBytes(decoded.data(), decoded.size());
}
