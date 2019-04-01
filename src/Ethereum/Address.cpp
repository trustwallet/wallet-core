// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "AddressChecksum.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <TrezorCrypto/sha3.h>

#include <cassert>

using namespace TW::Ethereum;

bool Address::isValid(const std::string& string) {
    if (string.size() != 42 || string[0] != '0' || string[1] != 'x') {
        return false;
    }
    const auto data = parse_hex(string);
    return Address::isValid(data);
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address data");
    }
    const auto data = parse_hex(string);
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const std::vector<uint8_t>& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type() != PublicKeyType::secp256k1Extended) {
        throw std::invalid_argument("Ethereum::Address needs an extended SECP256k1 public key.");
    }
    auto hash = std::array<uint8_t, Hash::sha256Size>();
    keccak_256(publicKey.bytes.data() + 1, publicKey.bytes.size() - 1, hash.data());
    std::copy(hash.end() - Address::size, hash.end(), bytes.begin());
}

std::string Address::string() const {
    return checksumed(*this, ChecksumType::eip55);
}
