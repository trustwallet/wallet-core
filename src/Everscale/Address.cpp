// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <charconv>
#include <system_error>

#include "Address.h"
#include "HexCoding.h"
#include "Wallet.h"
#include "WorkchainType.h"

using namespace TW;

namespace TW::Everscale {

bool Address::isValid(const std::string& string) noexcept {
    auto parsed = parseWorkchainId(string);
    if (!parsed.has_value())
        return false;

    auto [workchainId, pos] = *parsed;

    if (workchainId != WorkchainType::Basechain && workchainId != WorkchainType::Masterchain) {
        return false;
    }

    if (string.size() != pos + hexAddrLen) {
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
    auto [workchainId, pos] = *parsed;

    _workchainId = workchainId;

    const auto acc = parse_hex(string.substr(pos));
    std::copy(begin(acc), end(acc), begin(_address));
}

Address::Address(const PublicKey& publicKey, int8_t workchainId) {
    InitData initData(publicKey);
    auto [_, address] = initData.computeAddr(workchainId);

    _workchainId = workchainId;
    _address = address;
}

std::string Address::string() const {
    std::string address = std::to_string(_workchainId) + ":" + hex(_address);
    return address;
}

Address::MaybeWorkchainInfos Address::parseWorkchainId(const std::string& string) {
    if (auto pos = string.find(':'); pos != std::string::npos) {
        try {
            auto workchainId = static_cast<int8_t>(std::stoi(string.substr(0, pos)));
            return std::make_pair(workchainId, pos + 1);
        } catch (...) {
            // Do nothing and return empty value later
        }
    }

    return {};
}

} // namespace TW::Everscale
