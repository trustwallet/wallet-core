// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "CellBuilder.h"
#include "HexCoding.h"
#include "Wallet.h"
#include "WorkchainType.h"

using namespace TW;
using namespace TW::Everscale;

bool Address::isValid(const std::string& string) {
    auto parsed = parseWorkchainId(string);
    if (!parsed.has_value())
        return false;

    auto [workchainId, pos] = *parsed;

    if (workchainId != WorkchainType::Basechain && workchainId != WorkchainType::Masterchain) {
        return false;
    }

    if (string.size() != pos + 64) {
        return false;
    }

    std::string addr = string.substr(pos);
    return parse_hex(addr).size() == size;
}

Address::Address(const std::string& string) {
    if (!Address::isValid(string)) {
        throw std::invalid_argument("Invalid address string!");
    }

    auto parsed = parseWorkchainId(string);
    auto [wc, pos] = *parsed;

    wc_ = wc;

    const auto acc = parse_hex(string.substr(pos));
    std::copy(std::begin(acc), std::end(acc), std::begin(address_));
}

Address::Address(const PublicKey& publicKey, int8_t workchainId) {
    InitData initData(publicKey);
    auto [wc, address] = initData.computeAddr(workchainId);

    wc_ = wc;
    address_ = address;
}

std::string Address::string() const {
    std::string string = std::to_string(wc_) + ":" + hex(address_);
    return string;
}

std::optional<std::pair<int8_t, uint32_t>> Address::parseWorkchainId(const std::string& string) {
    int8_t workchainId = WorkchainType::Basechain;

    auto pos = string.find(':');
    if (pos != std::string::npos) {
        auto tmp = string.substr(0, pos);
        workchainId = static_cast<int8_t>(std::stoi(tmp));
        ++pos;
    } else {
        return {};
    }

    return std::make_pair(workchainId, pos);
}
