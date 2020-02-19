// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Hash.h"
#include "../HexCoding.h"
#include "../PrivateKey.h"

#include <TrezorCrypto/sha3.h>

using namespace TW;
using namespace TW::Icon;

static const std::string addressPrefix = "hx";
static const std::string contractPrefix = "cx";

bool Address::isValid(const std::string& string) {
    if (string.size() != Address::size * 2 + 2) {
        return false;
    }
    if (!std::equal(addressPrefix.begin(), addressPrefix.end(), string.begin()) &&
        !std::equal(contractPrefix.begin(), contractPrefix.end(), string.begin())) {
        return false;
    }
    return true;
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address data");
    }

    if (std::equal(addressPrefix.begin(), addressPrefix.end(), string.begin())) {
        type = TypeAddress;
    } else if (std::equal(contractPrefix.begin(), contractPrefix.end(), string.begin())) {
        type = TypeContract;
    } else {
        throw std::invalid_argument("Invalid address prefix");
    }

    const auto data = parse_hex(string.begin() + 2, string.end());
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey, enum AddressType type) : type(type) {
    auto hash = std::array<uint8_t, Hash::sha256Size>();
    sha3_256(publicKey.bytes.data() + 1, publicKey.bytes.size() - 1, hash.data());
    std::copy(hash.end() - Address::size, hash.end(), bytes.begin());
}

std::string Address::string() const {
    switch (type) {
    case TypeAddress:
        return addressPrefix + hex(bytes);
    case TypeContract:
        return contractPrefix + hex(bytes);
    default:
        return "";
    }
}
