// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBase32.h>

#include "../Base32.h"

#include <string>

using namespace TW;

TWData* TWBase32DecodeWithAlphabet(TWString* _Nonnull string, TWString* _Nonnull alphabet) {
    Data decodedOut;
    auto cppString = *reinterpret_cast<const std::string*>(string);
    const char* alphabetRaw = nullptr;
    if (TWStringSize(alphabet) > 0) {
        alphabetRaw = TWStringUTF8Bytes(alphabet);
    }
    auto result = Base32::decode(cppString, decodedOut, alphabetRaw);
    return result ? TWDataCreateWithData(&decodedOut) : nullptr;
}

TWData* _Nullable TWBase32Decode(TWString* _Nonnull string) {
    std::string empty;
    return TWBase32DecodeWithAlphabet(string, &empty);
}

TWString* _Nonnull TWBase32EncodeWithAlphabet(TWData* _Nonnull data, TWString* _Nonnull alphabet) {
    auto cppData = *reinterpret_cast<const Data*>(data);
    const char* alphabetRaw = nullptr;
    if (TWStringSize(alphabet) > 0) {
        alphabetRaw = TWStringUTF8Bytes(alphabet);
    }
    auto result = Base32::encode(cppData, alphabetRaw);
    return TWStringCreateWithUTF8Bytes(result.c_str());
}

TWString* _Nonnull TWBase32Encode(TWData* _Nonnull data) {
    std::string empty;
    return TWBase32EncodeWithAlphabet(data, &empty);
}
