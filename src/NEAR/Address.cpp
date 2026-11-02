// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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

/// Creates a NEAR address from an ED25519 public key.
Result<Address> Address::fromPublicKey(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeED25519 || publicKey.bytes.size() != size) {
        return Result<Address>::failure("Invalid public key type");
    }
    std::array<byte, size> addressBytes;
    std::copy(std::begin(publicKey.bytes), std::end(publicKey.bytes), std::begin(addressBytes));
    return Result<Address>::success(Address(addressBytes));
}

/// Returns a string representation of the NEAR address.
std::string Address::string() const {
    return hex(bytes);
}

} // namespace TW::NEAR
