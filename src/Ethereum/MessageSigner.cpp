// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "MessageSigner.h"
#include <proto/Ethereum.pb.h>
#include <proto/TransactionCompiler.pb.h>
#include "rust/Wrapper.h"
#include "TrustWalletCore/TWCoinType.h"

namespace TW::Ethereum {

std::string signMessageRust(const PrivateKey& privateKey, const std::string& message, Proto::MessageType msgType, MessageSigner::MaybeChainId chainId) {
    Proto::MessageSigningInput input;
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_message(message);
    input.set_message_type(msgType);

    if (chainId.has_value()) {
        input.mutable_chain_id()->set_chain_id(static_cast<uint64_t>(chainId.value()));
    }

    Rust::TWDataWrapper inputData(data(input.SerializeAsString()));
    Rust::TWDataWrapper outputPtr = Rust::tw_message_signer_sign(inputData.get(), TWCoinTypeEthereum);

    auto outputData = outputPtr.toDataOrDefault();
    if (outputData.empty()) {
        return {};
    }

    Proto::MessageSigningOutput output;
    output.ParseFromArray(outputData.data(), static_cast<int>(outputData.size()));

    if (output.error() != Common::Proto::SigningError::OK) {
        return {};
    }
    return output.signature();
}

Data messagePreImageHashRust(const std::string& message, Proto::MessageType msgType) {
    Proto::MessageSigningInput input;
    input.set_message(message);
    input.set_message_type(msgType);

    Rust::TWDataWrapper inputData(data(input.SerializeAsString()));
    Rust::TWDataWrapper outputPtr = Rust::tw_message_signer_pre_image_hashes(inputData.get(), TWCoinTypeEthereum);

    auto outputData = outputPtr.toDataOrDefault();
    if (outputData.empty()) {
        return {};
    }

    TxCompiler::Proto::PreSigningOutput output;
    output.ParseFromArray(outputData.data(), static_cast<int>(outputData.size()));

    if (output.error() != Common::Proto::SigningError::OK) {
        return {};
    }
    return data(output.data_hash());
}

bool verifyMessageRust(const PublicKey& publicKey, const std::string& message, const std::string& signature) {
    Proto::MessageVerifyingInput input;
    input.set_public_key(publicKey.bytes.data(), publicKey.bytes.size());
    input.set_message(message);
    input.set_signature(signature);

    Rust::TWDataWrapper inputData(data(input.SerializeAsString()));
    return Rust::tw_message_signer_verify(inputData.get(), TWCoinTypeEthereum);
}

std::string MessageSigner::signMessage(const PrivateKey& privateKey, const std::string& message, MessageType msgType, MaybeChainId chainId) {
    auto protoMsgType = Proto::MessageType::MessageType_legacy;
    switch (msgType) {
        case MessageType::Eip155: {
            protoMsgType = Proto::MessageType::MessageType_eip155;
            break;
        }
        case MessageType::ImmutableX: {
            protoMsgType = Proto::MessageType::MessageType_immutable_x;
            break;
        }
        default: {
            break;
        }
    }

    return signMessageRust(privateKey, message, protoMsgType, chainId);
}

std::string MessageSigner::signTypedData(const PrivateKey& privateKey, const std::string& data, MessageType msgType, MessageSigner::MaybeChainId chainId) {
    auto protoMsgType = Proto::MessageType::MessageType_typed;
    switch (msgType) {
        case MessageType::Eip155: {
            protoMsgType = Proto::MessageType::MessageType_typed_eip155;
            break;
        }
        default: {
            break;
        }
    }

    return signMessageRust(privateKey, data, protoMsgType, chainId);
}

bool MessageSigner::verifyMessage(const PublicKey& publicKey, const std::string& message, const std::string& signature) noexcept {
    return verifyMessageRust(publicKey, message, signature);
}

Data MessageSigner::messagePreImageHash(const std::string& message) noexcept {
    return messagePreImageHashRust(message, Proto::MessageType::MessageType_legacy);
}

Data MessageSigner::typedDataPreImageHash(const std::string& data) noexcept {
    return messagePreImageHashRust(data, Proto::MessageType::MessageType_typed);
}

void MessageSigner::prepareSignature(Data& signature, MessageType msgType, TW::Ethereum::MessageSigner::MaybeChainId chainId) noexcept {
    switch (msgType) {
        case MessageType::ImmutableX: {
            break;
        }
        case MessageType::Legacy: {
            signature[64] += 27;
            break;
        }
        case MessageType::Eip155: {
            auto id = chainId.value_or(0);
            signature[64] += 35 + id * 2;
            break;
        }
        default:
            break;
    }
}

} // namespace TW::Ethereum
