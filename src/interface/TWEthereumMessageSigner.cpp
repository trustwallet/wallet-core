// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumMessageSigner.h>

#include "Ethereum/MessageSigner.h"

namespace TW::internal {

TWString* _Nonnull TWEthereumMessageSignerSignCommon(const struct TWPrivateKey* _Nonnull privateKey, TWString* _Nonnull message, Ethereum::MessageType msgType, Ethereum::MessageSigner::MaybeChainId chainId = std::nullopt) {
    try {
        const auto signature = TW::Ethereum::MessageSigner::signMessage(privateKey->impl, TWStringUTF8Bytes(message), msgType, chainId);
        return TWStringCreateWithUTF8Bytes(signature.c_str());
    } catch (...) {
        return TWStringCreateWithUTF8Bytes("");
    }
}

TWString* _Nonnull TWEthereumMessageSignerSignTypedCommon(const struct TWPrivateKey* _Nonnull privateKey, TWString* _Nonnull message, Ethereum::MessageType msgType, Ethereum::MessageSigner::MaybeChainId chainId = std::nullopt) {
    try {
        const auto signature = TW::Ethereum::MessageSigner::signTypedData(privateKey->impl, TWStringUTF8Bytes(message), msgType, chainId);
        return TWStringCreateWithUTF8Bytes(signature.c_str());
    } catch (...) {
        return TWStringCreateWithUTF8Bytes("");
    }
}

} // namespace TW::internal

TWString* _Nonnull TWEthereumMessageSignerSignTypedMessage(const struct TWPrivateKey* _Nonnull privateKey, TWString* _Nonnull messageJson) {
    return TW::internal::TWEthereumMessageSignerSignTypedCommon(privateKey, messageJson, TW::Ethereum::MessageType::Legacy);
}

TWString* _Nonnull TWEthereumMessageSignerSignTypedMessageEip155(const struct TWPrivateKey* _Nonnull privateKey, TWString* _Nonnull messageJson, int chainId) {
    return TW::internal::TWEthereumMessageSignerSignTypedCommon(privateKey, messageJson, TW::Ethereum::MessageType::Eip155, static_cast<std::size_t>(chainId));
}

TWString* _Nonnull TWEthereumMessageSignerSignMessage(const struct TWPrivateKey* _Nonnull privateKey, TWString* _Nonnull message) {
    return TW::internal::TWEthereumMessageSignerSignCommon(privateKey, message, TW::Ethereum::MessageType::Legacy);
}

TWString* _Nonnull TWEthereumMessageSignerSignMessageImmutableX(const struct TWPrivateKey* _Nonnull privateKey, TWString* _Nonnull message) {
    return TW::internal::TWEthereumMessageSignerSignCommon(privateKey, message, TW::Ethereum::MessageType::ImmutableX);
}

TWString* _Nonnull TWEthereumMessageSignerSignMessageEip155(const struct TWPrivateKey* _Nonnull privateKey, TWString* _Nonnull message, int chainId) {
    return TW::internal::TWEthereumMessageSignerSignCommon(privateKey, message, TW::Ethereum::MessageType::Eip155, static_cast<std::size_t>(chainId));
}

bool TWEthereumMessageSignerVerifyMessage(const struct TWPublicKey* _Nonnull publicKey, TWString* _Nonnull message, TWString* _Nonnull signature) {
    return TW::Ethereum::MessageSigner::verifyMessage(publicKey->impl, TWStringUTF8Bytes(message), TWStringUTF8Bytes(signature));
}
