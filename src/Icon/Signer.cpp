// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include "../PrivateKey.h"
#include "../uint256.h"

#include <algorithm>
#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <map>
#include <sstream>

using namespace TW;
using namespace TW::Icon;

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

std::map<std::string, std::string> parameters(const Proto::SigningInput& input) {
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

std::string Signer::preImage(const Proto::SigningInput& input) noexcept {
    std::string txHash = "icx_sendTransaction";
    auto params = parameters(input);
    for (auto [key, value] : parameters(input)) {
        txHash += "." + key + "." + value;
    }
    return txHash;
}

Data Signer::sign(const Proto::SigningInput& input) noexcept {
    const auto hash = Hash::sha3_256(Signer::preImage(input));

    const auto key = PrivateKey(input.private_key());
    const auto signature = key.sign(hash);
    return Data(signature.begin(), signature.end());
}
