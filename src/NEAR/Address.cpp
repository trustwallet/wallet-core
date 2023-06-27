// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "Base58.h"
#include "HexCoding.h"

#include <stdexcept>

using namespace TW;

namespace TW::NEAR {

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
        return std::nullopt;
    }

    const Data& decoded = Base58::decode(string.substr(prefix.size()));
    if (decoded.size() != size + legacyChecksumSize) {
        return std::nullopt;
    }
    return Data(decoded.begin(), decoded.end() - legacyChecksumSize);
}

/// Initializes a NEAR address from a string representation.
Address::Address(const std::string& string) {
    const auto data = Address::decodeLegacyAddress(string);
    if (data.has_value()) {
        std::copy(std::begin(*data), std::end(*data), std::begin(bytes));
    } else {
        const auto parsed = parse_hex(string);
        if (parsed.size() != PublicKey::ed25519Size) {
            throw std::invalid_argument("Invalid address string!");
        }
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

} // namespace TW::NEAR
