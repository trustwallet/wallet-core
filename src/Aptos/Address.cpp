// Copyright © 2017-2022 Trust Wallet.
// Author: Clement Doumergue
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

namespace TW::Aptos {

bool Address::isValid(const std::string& string) {
    if (string.size() != 2 * Address::size) {
        return false;
    }
    const auto data = parse_hex(string);
    return isValid(data);
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }
    const auto data = parse_hex(string);
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const Data& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeED25519) {
        throw std::invalid_argument("Invalid public key type");
    }
    auto key_data = publicKey.bytes;
    append(key_data, 0x00);
    const auto data = Hash::sha3_256(key_data);
    std::copy(data.begin(), data.end(), bytes.begin());
}

std::string Address::string() const {
    return hex(bytes);
}

} // namespace TW::Aptos
