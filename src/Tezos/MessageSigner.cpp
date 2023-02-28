// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "HexCoding.h"
#include "Tezos/MessageSigner.h"

namespace TW::Tezos {

std::string inputToPayload(const std::string& input) {
    using namespace std::string_literals;
    auto bytes = data(input);
    size_t bytesLength = bytes.size();
    std::string addPadding = std::string(8 - std::to_string(bytesLength).size(), '0') + hex(uint64_t(bytesLength));
    std::string paddedBytesLength = addPadding.substr(addPadding.size() - 8);
    std::string payloadBytes = "05"s + "01"s + paddedBytesLength + hex(bytes);
    return payloadBytes;
}

std::string formatMessage(const std::string& message, const std::string& dAppUrl) {
    const std::string prefix = "Tezos Signed Message:";
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::ostringstream oss;
    oss << prefix << " " << dAppUrl << " ";
    oss << std::put_time(std::gmtime(&now_time), "%FT%T.") << std::setw(3) << std::setfill('0') << now_ms.count() << "Z";
    oss << " " << message;
    return oss.str();
}

} // namespace TW::Tezos
