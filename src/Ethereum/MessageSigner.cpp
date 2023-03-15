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

namespace TW::Ethereum::internal {

std::string commonSign(const PrivateKey& privateKey, const Data& signableMessage, MessageType msgType, TW::Ethereum::MessageSigner::MaybeChainId chainId) {
    auto data = privateKey.sign(signableMessage, TWCurveSECP256k1);
    switch (msgType) {
    case MessageType::ImmutableX:
        break;
    case MessageType::Legacy:
        data[64] += 27;
        break;
    case MessageType::Eip155:
        auto id = chainId.value_or(0);
        data[64] += 35 + id * 2;
        break;
    }
    return hex(data);
}

} // namespace TW::Ethereum::internal

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
    return internal::commonSign(privateKey, signableMessage, msgType, chainId);
}

std::string MessageSigner::signTypedData(const PrivateKey& privateKey, const std::string& data, MessageType msgType, MessageSigner::MaybeChainId chainId) {
    if (msgType == MessageType::Eip155 && nlohmann::json::accept(data)) {
        auto json = nlohmann::json::parse(data);
        if (json.contains("types") && json.at("types").contains("EIP712Domain")) {
            if (json.at("domain").at("chainId").get<std::size_t>() != *chainId) {
                return "EIP712 chainId is different than the current chainID.";
            }
        }
    }
    auto signableMessage = ABI::ParamStruct::hashStructJson(data);
    return internal::commonSign(privateKey, signableMessage, msgType, chainId);
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

} // namespace TW::Ethereum
