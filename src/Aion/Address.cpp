// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Address.h"
#include "../HexCoding.h"

namespace TW::Aion {

bool Address::isValid(const std::string& string) {
    const auto data = parse_hex(string);
    return Address::isValid(data);
}

Address::Address(const std::string& string) {
    const auto data = parse_hex(string);
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const std::vector<uint8_t>& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    Data publicKeyData(publicKey.bytes.begin(), publicKey.bytes.end());
    auto hash = TW::Hash::blake2b(publicKeyData, size);
    std::copy(hash.end() - size, hash.end(), bytes.begin());
    bytes[0] = 0xa0;
}

std::string Address::string() const {
    return "0x" + hex(bytes);
}

} // namespace TW::Aion
