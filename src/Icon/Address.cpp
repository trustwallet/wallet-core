// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Hash.h"
#include "../HexCoding.h"
#include "../PrivateKeySecp256k1.h"

#include <TrezorCrypto/sha3.h>

using namespace TW;
using namespace TW::Icon;

static const std::string addressPrefix = "hx";
static const std::string contractPrefix = "cx";

bool Address::isValid(const std::string& string) {
    if (string.size() != Address::size * 2 + 2) {
        return false;
    }
    if (!std::equal(addressPrefix.begin(), addressPrefix.end(), string.begin()) && !std::equal(contractPrefix.begin(), contractPrefix.end(), string.begin())) {
        return false;
    }
    return true;
}

Address::Address(const std::string& string) {
    assert(Address::isValid(string));

    if (std::equal(addressPrefix.begin(), addressPrefix.end(), string.begin())) {
        type = TWIconAddressTypeAddress;
    } else if (std::equal(contractPrefix.begin(), contractPrefix.end(), string.begin())) {
        type = TWIconAddressTypeContract;
    } else {
        assert(false && "Invalid address prefix");
        type = TWIconAddressTypeAddress;
    }

    const auto data = parse_hex(string.begin() + 2, string.end());
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const std::vector<uint8_t>& data, TWIconAddressType type) : type(type) {
    assert(Address::isValid(data));
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey, TWIconAddressType type) : type(type) {
    auto hash = std::array<uint8_t, Hash::sha256Size>();
    sha3_256(publicKey.bytes.data() + 1, PublicKey::uncompressedSize - 1, hash.data());
    std::copy(hash.end() - Address::size, hash.end(), bytes.begin());
}

std::string Address::string() const {
    switch (type) {
    case TWIconAddressTypeAddress:
        return addressPrefix + hex(bytes);
    case TWIconAddressTypeContract:
        return contractPrefix + hex(bytes);
    default:
        return "";
    }
}
