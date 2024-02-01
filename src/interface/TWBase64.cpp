// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWBase64.h>

#include "../Base64.h"

#include <string>

using namespace TW;

TWData* _Nullable TWBase64Decode(TWString* _Nonnull string) {
    auto cppString = *reinterpret_cast<const std::string*>(string);
    Data decodedOut = Base64::decode(cppString);
    return TWDataCreateWithData(&decodedOut);
}

TWData* _Nullable TWBase64DecodeUrl(TWString* _Nonnull string) {
    auto cppString = *reinterpret_cast<const std::string*>(string);
    Data decodedOut = Base64::decodeBase64Url(cppString);
    return TWDataCreateWithData(&decodedOut);
}

TWString *_Nonnull TWBase64Encode(TWData *_Nonnull data) {
    auto cppData = *reinterpret_cast<const Data*>(data);
    auto result = Base64::encode(cppData);
    return TWStringCreateWithUTF8Bytes(result.c_str());
}

TWString *_Nonnull TWBase64EncodeUrl(TWData *_Nonnull data) {
    auto cppData = *reinterpret_cast<const Data*>(data);
    auto result = Base64::encodeBase64Url(cppData);
    return TWStringCreateWithUTF8Bytes(result.c_str());
}
