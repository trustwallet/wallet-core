// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "MessageSigner.h"
#include <Ethereum/ABI/ParamStruct.h>
#include <nlohmann/json.hpp>
#include <sstream>

namespace TW::Ethereum {

Data MessageSigner::generateMessage(const std::string& message) {
    std::string prefix(1, MessageSigner::EthereumPrefix);
    std::stringstream ss;
    ss << prefix << MessageSigner::MessagePrefix << std::to_string(message.size()) << message;
    Data signableMessage = Hash::keccak256(data(ss.str()));
    return signableMessage;
}

std::string MessageSigner::signMessage(const PrivateKey& privateKey, const std::string& message, MessageType msgType, MaybeChainId chainId) {
    auto signableMessage = generateMessage(message);
    return signHash(privateKey, signableMessage, msgType, chainId);
}

std::string MessageSigner::signTypedData(const PrivateKey& privateKey, const std::string& data, MessageType msgType, MessageSigner::MaybeChainId chainId) {
    if (msgType == MessageType::Eip155 && nlohmann::json::accept(data)) {
        auto json = nlohmann::json::parse(data);
        if (json.contains("types") && json.at("types").contains("EIP712Domain")) {
            const auto& chainIdValue = json.at("domain").at("chainId");
            std::size_t actualChainId{0};
            if (chainIdValue.is_string()) {
                actualChainId = std::stoull(chainIdValue.get<std::string>());
            } else {
                actualChainId = chainIdValue.get<std::size_t>();
            }
            if (actualChainId != *chainId) {
                return "EIP712 chainId is different than the current chainID.";
            }
        }
    }
    auto signableMessage = ABI::ParamStruct::hashStructJson(data);
    return signHash(privateKey, signableMessage, msgType, chainId);
}

bool MessageSigner::verifyMessage(const PublicKey& publicKey, const std::string& message, const std::string& signature) noexcept {
    Data msg = generateMessage(message);
    //! If it's json && EIP712Domain then we hash the struct
    if (nlohmann::json::accept(message)) {
        auto json = nlohmann::json::parse(message);
        if (json.contains("types") && json.at("types").contains("EIP712Domain")) {
            msg = ABI::ParamStruct::hashStructJson(message);
        }
    }
    auto rawSignature = parse_hex(signature);
    auto recovered = publicKey.recover(rawSignature, msg);
    return recovered == publicKey && publicKey.verify(rawSignature, msg);
}

std::string MessageSigner::signHash(const PrivateKey& privateKey, const Data& signableMessage, MessageType msgType, TW::Ethereum::MessageSigner::MaybeChainId chainId) {
    auto data = privateKey.sign(signableMessage, TWCurveSECP256k1);
    prepareSignature(data, msgType, chainId);
    return hex(data);
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
