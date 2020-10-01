// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base58.h"
#include "HexCoding.h"
#include "Address.h"

#include <stdexcept>

using namespace TW;
using namespace TW::NEAR;

bool Address::isValid(const std::string& string) {
    const auto data = Address::decodeLegacyAddress(string);
    if (data.has_value()) {
        return true;
    } 
    const auto parsed = parse_hex(string);
    return parsed.size() == PublicKey::ed25519Size;
}

/// Decode and verifies the key data from a base58 string.
std::optional<Data> Address::decodeLegacyAddress(const std::string& string) {
    const auto prefix = std::string("NEAR");
    if (string.substr(0, prefix.size()) != prefix) {
        return {};
    }

    const Data& decoded = Base58::bitcoin.decode(string.substr(prefix.size()));
    return Data(decoded.begin(), decoded.end() - 4);
}

/// Initializes a NEAR address from a string representation.
Address::Address(const std::string& string) {
    const auto data = Address::decodeLegacyAddress(string);
    if (data.has_value()) {
        std::copy(std::begin(*data), std::end(*data), std::begin(bytes));
    } else {
        if (!Address::isValid(string)) {
             throw std::invalid_argument("Invalid address string!");
        }
        const auto parsed = parse_hex(string);
        std::copy(std::begin(parsed), std::end(parsed), std::begin(bytes));
    }
}

/// Initializes a NEAR address from a public key.
Address::Address(const PublicKey& publicKey) {
    // copy the raw, compressed key data
    auto data = publicKey.compressed().bytes;
    std::copy(std::begin(data), std::end(data), std::begin(bytes));
}

/// Returns a string representation of the NEAR address.
std::string Address::string() const {
    return hex(bytes);
}
