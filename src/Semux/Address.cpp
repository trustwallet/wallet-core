// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "stdint.h"
#include "../Hash.h"
#include "../HexCoding.h"

using namespace TW::Semux;

bool Address::isValid(const std::string &string) {
    if (string.size() != 42 || string[0] != '0' || string[1] != 'x') {
        return false;
    }
    const auto data = parse_hex(string);
    return Address::isValid(data);
}

Address::Address(const std::string &string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address data");
    }
    const auto data = parse_hex(string);
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey &publicKey) {
    if (publicKey.type != TWPublicKeyTypeED25519) {
        throw std::invalid_argument("Semux::Address needs ED25519 public key");
    }
    const auto data = TWX509EncodeED25519PublicKey(TWDataCreateWithBytes(
        reinterpret_cast<const uint8_t *>(publicKey.bytes.data()), publicKey.bytes.size()));
    const auto h256 = Hash::blake2b(*reinterpret_cast<const std::vector<uint8_t> *>(data), 32);
    const auto h160 = Hash::ripemd(h256);
    std::copy(h160.begin(), h160.end(), this->bytes.begin());
}

std::string Address::string() const {
    return "0x" + hex(bytes);
}
