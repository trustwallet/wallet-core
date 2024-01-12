// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <nlohmann/json.hpp>
#include <sstream>

#include <Tron/MessageSigner.h>
#include "HexCoding.h"


namespace TW::Tron {

Data generateMessage(const std::string& message) {
    std::string prefix(1, MessageSigner::TronPrefix);
    std::stringstream ss;
    ss << prefix << MessageSigner::MessagePrefix << message;
    Data signableMessage = Hash::keccak256(data(ss.str()));
    return signableMessage;
}

std::string MessageSigner::signMessage(const PrivateKey& privateKey, const std::string& message) {
    auto signableMessage = generateMessage(message);
    auto data = privateKey.sign(signableMessage, TWCurveSECP256k1);
    data[64] += 27;
    return hex(data);
}

bool MessageSigner::verifyMessage(const PublicKey& publicKey, const std::string& message, const std::string& signature) noexcept {
    Data msg = generateMessage(message);
    auto rawSignature = parse_hex(signature);
    auto recovered = publicKey.recover(rawSignature, msg);
    return recovered == publicKey && publicKey.verify(rawSignature, msg);
}

} // namespace TW::Ethereum
