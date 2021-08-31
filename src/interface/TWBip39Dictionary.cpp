// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBip39Dictionary.h>

#include "../HDWallet.h"

using namespace TW;


struct TWBip39Dictionary *_Nullable TWBip39DictionaryCreate(TWString *_Nonnull dictionaryString) {
    try {
        auto dict = Bip39Dictionary::prepareDictionary(TWStringUTF8Bytes(dictionaryString));
        if (!dict.first) {
            return nullptr;
        }
        return new TWBip39Dictionary{ dict.second };
    } catch (...) {
        return nullptr;
    }
}

void TWBip39DictionaryDelete(struct TWBip39Dictionary *_Nonnull dictionary) {
    delete dictionary;
}
