// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Signer.h"

#include "../Base64.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include "../PrivateKey.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <sstream>

namespace TW::Icon {

std::string to_hex(int64_t i) {
    std::stringstream ss;
    ss << "0x" << std::hex << i;
    return ss.str();
}

std::string to_hex(const std::string& n) {
    auto s = hex(n);
    auto start = std::find_if(s.begin(), s.end(), [](auto c) { return c != '0'; });
    if (start == s.end()) {
        return "0x0";
    }
    return "0x" + std::string(start, s.end());
}

std::map<std::string, std::string> Signer::parameters() const noexcept {
    auto params = std::map<std::string, std::string>();
    params["from"] = input.from_address();
    params["to"] = input.to_address();
    params["timestamp"] = to_hex(input.timestamp());
    params["nonce"] = to_hex(input.nonce());
    params["stepLimit"] = to_hex(input.step_limit());
    params["value"] = to_hex(input.value());
    params["nid"] = to_hex(input.network_id());
    params["version"] = "0x3";
    return params;
}

std::string Signer::preImage() const noexcept {
    std::string txHash = "icx_sendTransaction";
    const auto params = parameters();
    for (auto [key, value] : params) {
        txHash += "." + key + "." + value;
    }
    return txHash;
}

TW::Data Signer::hashImage(const Data& image) const {
    return Hash::sha3_256(image);
}

std::string Signer::encode(const Data& signature) const noexcept {
    auto json = nlohmann::json();
    json["from"] = input.from_address();
    json["to"] = input.to_address();
    json["timestamp"] = to_hex(input.timestamp());
    json["nonce"] = to_hex(input.nonce());
    json["stepLimit"] = to_hex(input.step_limit());
    json["value"] = to_hex(input.value());
    json["nid"] = to_hex(input.network_id());
    json["version"] = "0x3";
    json["signature"] = Base64::encode(signature);
    return json.dump();
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto signer = Signer(input);
    return signer.sign();
}

Proto::SigningOutput Signer::sign() const noexcept {
    const auto hash = Hash::sha3_256(Signer::preImage());

    const auto key = PrivateKey(input.private_key());
    const auto signature = key.sign(hash, TWCurveSECP256k1);

    auto output = Proto::SigningOutput();
    output.set_signature(signature.data(), signature.size());

    auto encoded = encode(Data(signature.begin(), signature.end()));
    output.set_encoded(encoded.data(), encoded.size());

    return output;
}

} // namespace TW::Icon
