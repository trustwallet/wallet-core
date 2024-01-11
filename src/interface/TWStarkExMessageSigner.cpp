// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWStarkExMessageSigner.h>

#include "StarkEx/MessageSigner.h"

TWString* _Nonnull TWStarkExMessageSignerSignMessage(const struct TWPrivateKey* _Nonnull privateKey, TWString* _Nonnull message) {
    try {
        const auto signature = TW::StarkEx::MessageSigner::signMessage(privateKey->impl, TWStringUTF8Bytes(message));
        return TWStringCreateWithUTF8Bytes(signature.c_str());
    } catch (...) {
        return TWStringCreateWithUTF8Bytes("");
    }
}

bool TWStarkExMessageSignerVerifyMessage(const struct TWPublicKey* _Nonnull publicKey, TWString* _Nonnull message, TWString* _Nonnull signature) {
    return TW::StarkEx::MessageSigner::verifyMessage(publicKey->impl, TWStringUTF8Bytes(message), TWStringUTF8Bytes(signature));
}
