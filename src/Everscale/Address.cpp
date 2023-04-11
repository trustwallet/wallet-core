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

using MaybeWorkchain = std::optional<std::pair<int8_t, std::string::size_type>>;

MaybeWorkchain parseWorkchainId(const std::string& string) {
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

bool Address::isValid(const std::string& string) noexcept {
    auto parsed = parseWorkchainId(string);
    if (!parsed.has_value()) {
        return false;
    }

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
    auto [parsedWorkchainId, pos] = *parsed;

    workchainId = parsedWorkchainId;

    const auto parsedHash = parse_hex(string.substr(pos));
    std::copy(begin(parsedHash), end(parsedHash), begin(hash));
}

Address::Address(const PublicKey& publicKey, int8_t workchainId)
    : Address(InitData(publicKey).computeAddr(workchainId)) {
}

std::string Address::string() const {
    std::string address = std::to_string(workchainId) + ":" + hex(hash);
    return address;
}

} // namespace TW::Everscale
