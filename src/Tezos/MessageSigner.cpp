// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "Base58.h"
#include "HexCoding.h"
#include "Tezos/MessageSigner.h"

namespace TW::Tezos {

static const Data gEdSigPrefix{9, 245, 205, 134, 18};
static const std::string gMsgPrefix{"Tezos Signed Message:"};

std::string MessageSigner::inputToPayload(const std::string& input) {
    using namespace std::string_literals;
    auto bytes = data(input);
    size_t bytesLength = bytes.size();
    std::string addPadding = std::string(8 - std::to_string(bytesLength).size(), '0') + hex(uint64_t(bytesLength));
    std::string paddedBytesLength = addPadding.substr(addPadding.size() - 8);
    std::string payloadBytes = "05"s + "01"s + paddedBytesLength + hex(bytes);
    return payloadBytes;
}

std::string MessageSigner::formatMessage(const std::string& message, const std::string& dAppUrl) {
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::ostringstream oss;
    oss << gMsgPrefix << " " << dAppUrl << " ";
    oss << std::put_time(std::gmtime(&now_time), "%FT%T.") << std::setw(3) << std::setfill('0') << now_ms.count() << "Z";
    oss << " " << message;
    return oss.str();
}

std::string MessageSigner::signMessage(const PrivateKey& privateKey, const std::string& message) {
    auto signature = privateKey.sign(Hash::blake2b(parse_hex(message), 32), TWCurveED25519);
    return Base58::encodeCheck(concat(gEdSigPrefix, signature));
}

bool MessageSigner::verifyMessage(const PublicKey& publicKey, const std::string& message, const std::string& signature) noexcept {
    auto decoded = Base58::decodeCheck(signature);
    auto rawSignature = subData(decoded, gEdSigPrefix.size());
    auto msg = Hash::blake2b(parse_hex(message), 32);
    return publicKey.verify(rawSignature, msg);
}

} // namespace TW::Tezos
