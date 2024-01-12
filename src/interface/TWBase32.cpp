// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWBase32.h>

#include "../Base32.h"

#include <string>

using namespace TW;

TWData* TWBase32DecodeWithAlphabet(TWString* _Nonnull string, TWString* _Nullable alphabet) {
    Data decodedOut;
    auto cppString = *reinterpret_cast<const std::string*>(string);
    const char* alphabetRaw = nullptr;
    if (alphabet != nullptr) {
        alphabetRaw = TWStringUTF8Bytes(alphabet);
    }
    auto result = Base32::decode(cppString, decodedOut, alphabetRaw);
    return result ? TWDataCreateWithData(&decodedOut) : nullptr;
}

TWData* _Nullable TWBase32Decode(TWString* _Nonnull string) {
    return TWBase32DecodeWithAlphabet(string, nullptr);
}

TWString* _Nonnull TWBase32EncodeWithAlphabet(TWData* _Nonnull data, TWString* _Nullable alphabet) {
    auto cppData = *reinterpret_cast<const Data*>(data);
    const char* alphabetRaw = nullptr;
    if (alphabet != nullptr) {
        alphabetRaw = TWStringUTF8Bytes(alphabet);
    }
    auto result = Base32::encode(cppData, alphabetRaw);
    return TWStringCreateWithUTF8Bytes(result.c_str());
}

TWString* _Nonnull TWBase32Encode(TWData* _Nonnull data) {
    return TWBase32EncodeWithAlphabet(data, nullptr);
}
