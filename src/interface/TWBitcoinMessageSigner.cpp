// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBitcoinMessageSigner.h>

#include "Bitcoin/MessageSigner.h"

TWString* _Nonnull TWBitcoinMessageSignerSignMessage(const struct TWPrivateKey* _Nonnull privateKey, TWString* _Nonnull address, TWString* _Nonnull message) {
    try {
        const auto signature = TW::Bitcoin::MessageSigner::signMessage(privateKey->impl, TWStringUTF8Bytes(address), TWStringUTF8Bytes(message), true);
        return TWStringCreateWithUTF8Bytes(signature.c_str());
    } catch (...) {
        return TWStringCreateWithUTF8Bytes("");
    }
}

bool TWBitcoinMessageSignerVerifyMessage(TWString* _Nonnull address, TWString* _Nonnull message, TWString* _Nonnull signature) {
    return TW::Bitcoin::MessageSigner::verifyMessage(TWStringUTF8Bytes(address), TWStringUTF8Bytes(message), TWStringUTF8Bytes(signature));
}
