// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "Ethereum/Address.h"
#include "../Hash.h"
#include "../HexCoding.h"

const std::string prefix = "ronin:";
using namespace TW::Ronin;

bool Address::isValid(const std::string& string) {
    // check prefix
    if (string.compare(0, prefix.length(), prefix) != 0) {
        return false;
    }

    const auto suffix = string.substr(prefix.length());
    const auto data = parse_hex(suffix);
    return Ethereum::Address::isValid(data);
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address data");
    }
    const auto suffix = string.substr(prefix.length());
    const auto data = parse_hex(suffix);
    std::copy(data.begin(), data.end(), bytes.begin());
}

std::string Address::string() const {
    return prefix + hex(bytes);
}
