// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWBitcoinMessageSigner.h>

#include "Bitcoin/MessageSigner.h"

TWString* _Nonnull TWBitcoinMessageSignerSignMessage(const struct TWPrivateKey* _Nonnull privateKey, TWString* _Nonnull address, TWString* _Nonnull message) {
    try {
        // Non-throwing: WASM builds cannot catch exceptions.
        const auto signature = TW::Bitcoin::MessageSigner::trySignMessage(privateKey->impl, TWStringUTF8Bytes(address), TWStringUTF8Bytes(message), true);
        return TWStringCreateWithUTF8Bytes(signature.value_or("").c_str());
    } catch (...) {
        return TWStringCreateWithUTF8Bytes("");
    }
}

bool TWBitcoinMessageSignerVerifyMessage(TWString* _Nonnull address, TWString* _Nonnull message, TWString* _Nonnull signature) {
    return TW::Bitcoin::MessageSigner::verifyMessage(TWStringUTF8Bytes(address), TWStringUTF8Bytes(message), TWStringUTF8Bytes(signature));
}
