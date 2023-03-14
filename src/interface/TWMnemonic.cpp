// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
