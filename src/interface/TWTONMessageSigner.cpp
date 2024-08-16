// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TrustWalletCore/TWTONMessageSigner.h"
#include "rust/Wrapper.h"
#include "PrivateKey.h"

using namespace TW;

TWString *_Nullable TWTONMessageSignerSignMessage(struct TWPrivateKey *_Nonnull privateKey, TWString* _Nonnull message) {
    auto* privateKeyRustRaw = Rust::tw_private_key_create_with_data(privateKey->impl.bytes.data(), privateKey->impl.bytes.size());
    const auto privateKeyRust = std::shared_ptr<Rust::TWPrivateKey>(privateKeyRustRaw, Rust::tw_private_key_delete);

    Rust::TWStringWrapper messageRust = TWStringUTF8Bytes(message);
    Rust::TWStringWrapper signature = Rust::tw_ton_message_signer_sign_message(privateKeyRust.get(), messageRust.get());

    if (!signature) {
        return nullptr;
    }
    return TWStringCreateWithUTF8Bytes(signature.c_str());
}
