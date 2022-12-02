// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "EIP191.h"
#include "HexCoding.h"
#include <iostream>
#include <sstream>

namespace TW::Ethereum::internal {

Data generateMessage(const std::string& message) {
    std::string prefix(1, MessageSigner::EthereumPrefix);
    std::stringstream ss;
    ss << prefix << MessageSigner::MessagePrefix << std::to_string(message.size()) << message;
    Data signableMessage = Hash::keccak256(data(ss.str()));
    return signableMessage;
}

} // namespace TW::Ethereum::internal

namespace TW::Ethereum {

std::string MessageSigner::signMessage(const PrivateKey& privateKey, const std::string& message) {
    auto signableMessage = internal::generateMessage(message);
    return hex(privateKey.sign(signableMessage, TWCurveSECP256k1));
}

bool MessageSigner::verifyMessage(const PublicKey& publicKey, const std::string& message, const std::string& signature) noexcept {
    auto msg = internal::generateMessage(message);
    auto rawSignature = parse_hex(signature);
    auto recovered = publicKey.recover(rawSignature, msg);
    return recovered == publicKey && publicKey.verify(rawSignature, msg);
}

} // namespace TW::Ethereum
