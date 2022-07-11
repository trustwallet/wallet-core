// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBase32.h>

#include "../Base32.h"

#include <string>

using namespace TW;

TWData* TWBase32DecodeWithAlphabet(TWString* string, TWString* alphabet) {
    Data decodedOut;
    auto cppString = *reinterpret_cast<const std::string*>(string);
    const char* alphabetRaw = nullptr;
    if (alphabet != nullptr) {
        alphabetRaw = TWStringUTF8Bytes(alphabet);
    }
    if (!Base32::decode(cppString, decodedOut, alphabetRaw)) {
        return nullptr;
    }
    return TWDataCreateWithData(&decodedOut);
}

TWData* TWBase32Decode(TWString* string) {
    return TWBase32DecodeWithAlphabet(string, nullptr);
}
