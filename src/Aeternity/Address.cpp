// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "Identifiers.h"
#include <Base58.h>
#include <Coin.h>
#include <HexCoding.h>

using namespace TW::Aeternity;

/// Determines whether a string makes a valid address.
/// copied from \see https://github.com/aeternity/aepp-sdk-js/blob/develop/es/utils/crypto.js
bool TW::Aeternity::Address::isValid(const std::string &string) {
    if (string.empty()) {
        return false;
    } else {
        auto prefixSize = Identifiers::prefixAccountPubkey.size();
        auto type = string.substr(0, prefixSize);
        auto payload = string.substr(prefixSize, string.size() - 1);
        return checkType(type) && checkPayload(payload);
    }
}

/// Initializes an address from a public key.
TW::Aeternity::Address::Address(const PublicKey &publicKey) {
    if (publicKey.type != TWPublicKeyTypeED25519) {
        throw std::invalid_argument("Invalid public key type");
    }

    bytes = publicKey.bytes;
}

/// Initializes an address from a string representation.
TW::Aeternity::Address::Address(const std::string &string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address");
    }

    auto payload = string.substr(Identifiers::prefixTransaction.size(), string.size());
    bytes = Base58::bitcoin.decodeCheck(payload);
}

/// Returns a string representation of the Bravo address.
std::string Address::string() const {
    return Identifiers::prefixAccountPubkey + Base58::bitcoin.encodeCheck(bytes);
}

bool Address::checkType(const std::string &type) {
    return type == Identifiers::prefixAccountPubkey;
}

bool Address::checkPayload(const std::string &payload) {
    unsigned long base58 = Base58::bitcoin.decodeCheck(payload).size();
    return base58 == size;
}
