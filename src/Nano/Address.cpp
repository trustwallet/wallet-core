// Copyright © 2019 Mart Roosmaa.
// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Address.h"
#include <TrezorCrypto/nano.h>

#include <string>

namespace TW::Nano {

const std::string kPrefixNano{"nano_"};
const std::string kPrefixXrb{"xrb_"};

bool Address::isValid(const std::string& address) {
    bool valid = false;

    valid = nano_validate_address(
        kPrefixNano.c_str(), kPrefixNano.length(),
        address.c_str(), address.length(),
        nullptr);
    if (!valid) {
        valid = nano_validate_address(
            kPrefixXrb.c_str(), kPrefixXrb.length(),
            address.c_str(), address.length(),
            nullptr);
    }

    return valid;
}

Address::Address(const std::string& address) {
    bool valid = false;

    valid = nano_validate_address(
        kPrefixNano.c_str(), kPrefixNano.length(),
        address.c_str(), address.length(),
        bytes.data());

    if (!valid) {
        valid = nano_validate_address(
            kPrefixXrb.c_str(), kPrefixXrb.length(),
            address.c_str(), address.length(),
            bytes.data());
    }

    // Ensure address is valid
    if (!valid) {
        throw std::invalid_argument("Invalid address data");
    }
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeED25519Blake2b) {
        throw std::invalid_argument("Invalid public key type");
    }

    auto keyBytes = publicKey.bytes;
    std::copy(keyBytes.begin(), keyBytes.begin() + 32, bytes.data());
}

std::string Address::string() const {
    std::array<char, 5 + 60 + 1 + 1> out = {0};

    size_t count = nano_get_address(
        bytes.data(),
        kPrefixNano.c_str(), kPrefixNano.length(),
        out.data(), out.size());
    // closing \0
    assert(count < out.size());
    out[count] = 0;
    return {out.data()};
}

} // namespace TW::Nano
