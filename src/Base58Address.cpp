// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base58Address.h"

#include "Base58.h"

using namespace TW;

bool Base58Address::isValid(const std::string& string) {
    const auto decoded = Base58::bitcoin.decodeCheck(string);
    if (decoded.size() != Base58Address::size) {
        return false;
    }
    return true;
}

bool Base58Address::isValid(const std::string& string, const Data& validPrefixes) {
    const auto decoded = Base58::bitcoin.decodeCheck(string);
    if (decoded.size() != Base58Address::size) {
        return false;
    }
    if (std::find(validPrefixes.begin(), validPrefixes.end(), decoded[0]) == validPrefixes.end()) {
        return false;
    }
    return true;
}

Base58Address::Base58Address(const std::string& string) {
    const auto decoded = Base58::bitcoin.decodeCheck(string);
    if (decoded.size() != Base58Address::size) {
        throw std::invalid_argument("Invalid address string");
    }

    std::copy(decoded.begin(), decoded.end(), bytes.begin());
}

Base58Address::Base58Address(const Data& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address key data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Base58Address::Base58Address(const PublicKey& publicKey, uint8_t prefix) {
    if (publicKey.type() != PublicKeyType::secp256k1) {
        throw std::invalid_argument("Bitcoin::Address needs a compressed SECP256k1 public key.");
    }
    const auto data = publicKey.hash({prefix}, Hash::sha256ripemd);
    std::copy(data.begin(), data.end(), bytes.begin());
}

std::string Base58Address::string() const {
    return Base58::bitcoin.encodeCheck(bytes);
}
