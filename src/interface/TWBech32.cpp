// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWBech32.h>

#include "../Bech32.h"

#include <string>

using namespace TW;

static TWString *_Nonnull encodeGeneric(TWString* _Nonnull hrp, TWData *_Nonnull data, const Bech32::ChecksumVariant variant) {
    const auto cppHrp = *static_cast<const std::string*>(hrp);
    const auto cppData = *static_cast<const Data*>(data);
    Data enc;
    if (!Bech32::convertBits<8, 5, true>(enc, cppData)) {
        return TWStringCreateWithUTF8Bytes("");
    }
    const auto result = Bech32::encode(cppHrp, enc, variant);
    return TWStringCreateWithUTF8Bytes(result.c_str());
}

static TWData *_Nullable decodeGeneric(TWString *_Nonnull string, const Bech32::ChecksumVariant variant) {
    const auto cppString = *static_cast<const std::string*>(string);
    const auto decoded = Bech32::decode(cppString);

    const auto data = std::get<1>(decoded);
    if (data.empty()) { // Failed to decode
        return nullptr;
    }

    if (std::get<2>(decoded) != variant) { // Wrong ChecksumVariant
        return nullptr;
    }

    // Bech bits conversion
    Data conv;
    if (!Bech32::convertBits<5, 8, false>(conv, data)) {
        return nullptr;
    }

    return TWDataCreateWithBytes(conv.data(), conv.size());
}

TWString *_Nonnull TWBech32Encode(TWString* _Nonnull hrp, TWData *_Nonnull data) {
    return encodeGeneric(hrp, data, Bech32::ChecksumVariant::Bech32);
}

TWString *_Nonnull TWBech32EncodeM(TWString* _Nonnull hrp, TWData *_Nonnull data) {
    return encodeGeneric(hrp, data, Bech32::ChecksumVariant::Bech32M);
}

TWData *_Nullable TWBech32Decode(TWString *_Nonnull string) {
    return decodeGeneric(string, Bech32::ChecksumVariant::Bech32);
}

TWData *_Nullable TWBech32DecodeM(TWString *_Nonnull string) {
        return decodeGeneric(string, Bech32::ChecksumVariant::Bech32M);
}
