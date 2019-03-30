// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TAddress.h"

#include "../Base58.h"

#include <TrezorCrypto/ecdsa.h>

#include <cassert>

using namespace TW::Zcash;

bool TAddress::isValid(const std::string& string) {
    const auto decoded = Base58::bitcoin.decodeCheck(string);
    if (decoded.size() != TAddress::size) {
        return false;
    }

    return true;
}

bool TAddress::isValid(const std::string& string, const std::vector<byte>& validPrefixes) {
    const auto decoded = Base58::bitcoin.decodeCheck(string);
    if (decoded.size() != TAddress::size) {
        return false;
    }

    if (std::find(validPrefixes.begin(), validPrefixes.end(), decoded[1]) == validPrefixes.end()) {
        return false;
    }

    return true;
}

TAddress::TAddress(const std::string& string) {
    const auto decoded = Base58::bitcoin.decodeCheck(string);
    if (decoded.size() != TAddress::size) {
        throw std::invalid_argument("Invalid address data");
    }

    std::copy(decoded.begin(), decoded.end(), bytes.begin());
}

TAddress::TAddress(const std::vector<uint8_t>& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address key data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

TAddress::TAddress(const PublicKey& publicKey, uint8_t prefix) {
    bytes[0] = 0x1c;
    bytes[1] = prefix;
    ecdsa_get_pubkeyhash(publicKey.bytes.data(), HASHER_SHA2_RIPEMD, bytes.data() + 2);
}

std::string TAddress::string() const {
    return Base58::bitcoin.encodeCheck(bytes.data(), bytes.data() + TAddress::size);
}
