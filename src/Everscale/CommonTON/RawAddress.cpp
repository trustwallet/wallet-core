// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <limits>
#include <climits>

#include "RawAddress.h"

#include "HexCoding.h"
#include "WorkchainType.h"

using MaybeWorkchain = std::optional<std::pair<int8_t, std::string::size_type>>;

namespace TW::CommonTON {

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

static std::optional<int8_t> parse_int8(char const *s) {
    auto value = parse_long(s);
    if (!value.has_value()) {
        return std::nullopt;
    }
    if (value.value() <= static_cast<long>(std::numeric_limits<int8_t>::max()) && value.value() >= static_cast<long>(std::numeric_limits<int8_t>::min())) {
        return value;
    } else {
        return std::nullopt;
    }
}

static MaybeWorkchain parseWorkchainId(const std::string& string) {
    if (auto pos = string.find(':'); pos != std::string::npos) {
        std::string workchain_string = string.substr(0, pos);
        auto workchain_id = parse_int8(workchain_string.c_str());
        if (workchain_id.has_value()) {
            return std::make_pair(workchain_id.value(), pos + 1);
        }
    }
    return {};
}

bool RawAddress::isValid(const std::string& string) {
    auto parsed = parseWorkchainId(string);
    if (!parsed.has_value()) {
        return false;
    }

    auto [workchainId, pos] = *parsed;

    if (workchainId != WorkchainType::Basechain && workchainId != WorkchainType::Masterchain) {
        return false;
    }

    if (string.size() != pos + AddressData::hexAddrLen) {
        return false;
    }

    std::string addr = string.substr(pos);
    if (!is_hex_encoded(addr)) {
        return false;
    }
    return parse_hex(addr).size() == AddressData::size;
}

AddressData RawAddress::splitAddress(const std::string& address) {
    auto parsed = parseWorkchainId(address);
    auto [parsedWorkchainId, pos] = *parsed;

    auto workchainId = parsedWorkchainId;

    const auto parsedHash = parse_hex(address.substr(pos));

    std::array<byte, AddressData::size> hash{};
    std::copy(begin(parsedHash), end(parsedHash), begin(hash));

    return AddressData(workchainId, hash);
}

std::string RawAddress::to_string(const AddressData& addressData) {
    std::string address = std::to_string(addressData.workchainId) + ":" + hex(addressData.hash);
    return address;
}

} // namespace TW::CommonTON
