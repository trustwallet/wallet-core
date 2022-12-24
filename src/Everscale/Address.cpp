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


static std::optional<long> parse_long(char const *s)
{
    char *end;
    auto previous_errno = errno;
    errno = 0;
    long l = strtol(s, &end, 10);
    if (errno == ERANGE && l == LONG_MAX) {
        errno = previous_errno;
        return {}; // Overflow
    }
    if (errno == ERANGE && l == LONG_MIN) {
        errno = previous_errno;
        return {}; // Underflow
    }
    if (*s == '\0' || *end != '\0') {
        errno = previous_errno;
        return {}; // Inconvertible
    }
    errno = previous_errno;
    return l;
}


using MaybeWorkchain = std::optional<std::pair<int8_t, std::string::size_type>>;

MaybeWorkchain parseWorkchainId(const std::string& string) {
    if (auto pos = string.find(':'); pos != std::string::npos) {
        std::string workchain_string = string.substr(0, pos);
        auto workchain_id = parse_long(workchain_string.c_str());
        if (workchain_id.has_value()) {
            return std::make_pair(workchain_id.value(), pos + 1);
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
