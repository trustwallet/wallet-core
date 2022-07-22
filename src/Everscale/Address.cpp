// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "HexCoding.h"

using namespace TW::Everscale;

bool Address::isValid(const std::string& string) {
    auto parsed_wc = parseWorkchainId(string);
    if (!parsed_wc.has_value())
        return false;

    auto [wc, pos] = *parsed_wc;

    if (wc != basechainId && wc != masterchainId) {
        return false;
    }

    if (string.size() != pos + 64) {
        return false;
    }

    std::string s = string.substr(pos);
    const auto parsed = parse_hex(s);
    return parsed.size() == size;
}

Address::Address(const std::string& string) {
    if (!Address::isValid(string)) {
        throw std::invalid_argument("Invalid address string!");
    }

    auto parsed_wc = parseWorkchainId(string);
    auto [wc, pos] = *parsed_wc;

    workchainId = wc;

    std::string addr = string.substr(pos);
    const auto parsed = parse_hex(addr);
    std::copy(std::begin(parsed), std::end(parsed), std::begin(bytes));
}

Address::Address(const PublicKey& publicKey) {
    // TODO: Finalize implementation
}

std::string Address::string() const {
    std::string string = std::to_string(workchainId) + ":" + hex(bytes);
    return string;
}

std::optional<std::pair<int8_t, int32_t>> Address::parseWorkchainId(const std::string& string) {
    auto wc = basechainId;
    auto pos = string.find(':');
    if (pos != std::string::npos) {
        auto tmp = string.substr(0, pos);
        wc = static_cast<int8_t>(std::stoi(tmp));
        ++pos;
    } else {
        pos = 0;
    }

    return std::make_pair(wc, pos);
}
