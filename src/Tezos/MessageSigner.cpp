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
static constexpr uint8_t gMessageWatermark{0x05};

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

/// Decodes a hex message payload that must start with the 0x05 watermark.
/// Only off-chain message payloads (0x05) are signed; for operations (0x03) use the Tezos transaction signer.
static Result<Data> decodeMessagePayload(const std::string& message) {
    auto decoded = parse_hex_checked(message);
    if (decoded.isFailure()) {
        return decoded;
    }
    auto payload = decoded.payload();
    if (payload.empty() || payload[0] != gMessageWatermark) {
        return Result<Data>::failure("Message must start with the 0x05 watermark");
    }
    return Result<Data>::success(std::move(payload));
}

Result<std::string> MessageSigner::signMessage(const PrivateKey& privateKey, const std::string& message) {
    const auto payload = decodeMessagePayload(message);
    if (payload.isFailure()) {
        return Result<std::string>::failure(payload.error());
    }
    auto signature = privateKey.sign(Hash::blake2b(payload.payload(), 32), TWCurveED25519);
    return Result<std::string>::success(Base58::encodeCheck(concat(gEdSigPrefix, signature)));
}

bool MessageSigner::verifyMessage(const PublicKey& publicKey, const std::string& message, const std::string& signature) noexcept {
    try {
        const auto payload = decodeMessagePayload(message);
        if (payload.isFailure()) {
            return false;
        }
        auto decoded = Base58::decodeCheck(signature);
        auto rawSignature = subData(decoded, gEdSigPrefix.size());
        auto msg = Hash::blake2b(payload.payload(), 32);
        return publicKey.verify(rawSignature, msg);
    } catch (...) {
        return false;
    }
}

} // namespace TW::Tezos
