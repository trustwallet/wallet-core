// Copyright © 2019 Mart Roosmaa.
// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Hash.h"
#include "../HexCoding.h"

#include <string>

using namespace TW::Vite;

const std::string kPrefixVite{"vite_"};

bool Address::isValid(const std::string& address) {
    return isValid(address, false);
}

bool Address::isValid(const std::string& address, bool isContract = false) {
    if (address.length() != 55) return false;

    if (address.substr(0, 5) != kPrefixVite) return false;

    auto checksum = Hash::blake2b(
      parse_hex(
        address.substr(5, 40) + (isContract ? "01" : "00")
      ).data(),
      21,
      5
    );

    if (address.substr(45) != hex(checksum)) return false;

    return true;
}

//Address::Address(const std::string& address) {
    // bytes = parse_hex(address.substr(5));
//}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeED25519Blake2b) {
        throw std::invalid_argument("Invalid public key type");
    }

    auto addr = Hash::blake2b(publicKey.bytes.data(), 32, 20);
    append(addr, 0x00);
    auto checksum = Hash::blake2b(addr.data(), 21, 5);
    addr.pop_back();
    append(addr, checksum);
    std::copy(addr.begin(), addr.begin() + 25, bytes.data());
//    this->bytes = addr.data();
}

std::string Address::string() const {
    return kPrefixVite + hex(bytes);
}
