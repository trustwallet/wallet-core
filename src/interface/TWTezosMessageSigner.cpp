// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWTezosMessageSigner.h>
#include "Tezos/MessageSigner.h"

bool TWTezosMessageSignerVerifyMessage(const struct TWPublicKey* _Nonnull publicKey, TWString* _Nonnull message, TWString* _Nonnull signature) {
    return TW::Tezos::MessageSigner::verifyMessage(publicKey->impl, TWStringUTF8Bytes(message), TWStringUTF8Bytes(signature));
}

TWString* _Nullable TWTezosMessageSignerSignMessage(const struct TWPrivateKey* _Nonnull privateKey, TWString* _Nonnull message) {
    try {
        const auto signature = TW::Tezos::MessageSigner::signMessage(privateKey->impl, TWStringUTF8Bytes(message));
        if (signature.isFailure()) {
            return nullptr;
        }
        return TWStringCreateWithUTF8Bytes(signature.payload().c_str());
    } catch (...) {
        return nullptr;
    }
}

TWString* TWTezosMessageSignerFormatMessage(TWString* _Nonnull message, TWString* _Nonnull url) {
    const auto formatedMessage = TW::Tezos::MessageSigner::formatMessage(TWStringUTF8Bytes(message), TWStringUTF8Bytes(url));
    return TWStringCreateWithUTF8Bytes(formatedMessage.c_str());
}

TWString* TWTezosMessageSignerInputToPayload(TWString* message) {
    const auto payload = TW::Tezos::MessageSigner::inputToPayload(TWStringUTF8Bytes(message));
    return TWStringCreateWithUTF8Bytes(payload.c_str());
}
