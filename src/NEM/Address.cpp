// Copyright © 2019 Mart Roosmaa.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include <TrezorCrypto/nem.h>

#include <string>

using namespace TW::NEM;

bool Address::isValid(const std::string& address) {
    bool valid = false;

    valid = nem_validate_address(address.c_str(), 0);
    if (!valid) {
        valid = nem_validate_address(address.c_str(), 0);
    }

    return valid;
}

Address::Address(const std::string& address) {
    bool valid = false;

    valid = nem_validate_address(address.c_str(), 0);

    if (!valid) {
        valid = nem_validate_address(address.c_str(), 0);
    }

    // Ensure address is valid
    if (!valid) {
        throw std::invalid_argument("Invalid address data");
    }
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeED25519) {
        throw std::invalid_argument("Invalid public key type");
    }

    auto keyBytes = publicKey.bytes;
    std::copy(keyBytes.begin(), keyBytes.begin() + 32, bytes.data());
}

std::string Address::string() const {
    std::array<char, 5 + 60 + 1> out = {0};

    size_t count = nem_get_address(bytes.data(), 0, address);
    out[count] = 0;
    return std::string(out.data());
}
