// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "../HexCoding.h"

using namespace TW::Top;

bool Address::isValid(const std::string& string) {
    // TODO: Finalize implementation
    if (string.size() != Address::size*2 + TOP_PREFIX.size() || string.substr(0, TOP_PREFIX.size()) != TOP_PREFIX) {
        return false;
    }
    const auto data = parse_hex(std::string("0x") + string.substr(TOP_PREFIX.size()));
    return Address::isValid(data);    
}

Address::Address(const std::string& string) {
    // TODO: Finalize implementation
    const auto data = parse_hex(std::string("0x") + string.substr(TOP_PREFIX.size()));
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());    
}

Address::Address(const PublicKey& publicKey) {
    // TODO: Finalize implementation
    const auto data = publicKey.hash({}, static_cast<Hash::HasherSimpleType>(Hash::keccak256), true);
    std::copy(data.end() - Address::size, data.end(), bytes.begin());
}

std::string Address::string() const {
    // TODO: Finalize implementation
    return TOP_PREFIX + hex(bytes);
}
