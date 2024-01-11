// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWMnemonic.h>

#include "../Mnemonic.h"

using namespace TW;

bool TWMnemonicIsValid(TWString *_Nonnull mnemonic) {
    return Mnemonic::isValid(TWStringUTF8Bytes(mnemonic));
}

bool TWMnemonicIsValidWord(TWString *_Nonnull word) {
    return Mnemonic::isValidWord(TWStringUTF8Bytes(word));
}

TWString* _Nonnull TWMnemonicSuggest(TWString *_Nonnull prefix) {
    auto result = Mnemonic::suggest(std::string(TWStringUTF8Bytes(prefix)));
    return TWStringCreateWithUTF8Bytes(result.c_str());
}
