// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Base58.h"
#include "../Hash.h"

#include <cassert>
#include <stdexcept>

using namespace TW::Tron;

bool Address::isValid(const std::string& string) {
    const auto decoded = Base58::bitcoin.decodeCheck(string);
    if (decoded.size() != Address::size) {
        return false;
    }

    if (decoded[0] != prefix) {
        return false;
    }

    return true;
}

Address::Address(const std::string& string) {
    const auto decoded = Base58::bitcoin.decodeCheck(string);
    if (decoded.size() != Address::size || decoded[0] != prefix) {
        throw std::invalid_argument("Invalid address string");
    }

    std::copy(decoded.begin(), decoded.end(), bytes.begin());
}

Address::Address(const Data& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type() != PublicKeyType::secp256k1Extended) {
        throw std::invalid_argument("Invalid public key type");
    }
    const auto pkdata = Data(publicKey.bytes.begin() + 1, publicKey.bytes.end());
    const auto keyhash = Hash::keccak256(pkdata);
    bytes[0] = prefix;
    std::copy(keyhash.end() - size + 1, keyhash.end(), bytes.begin() + 1);
}

std::string Address::string() const {
    return Base58::bitcoin.encodeCheck(bytes);
}
