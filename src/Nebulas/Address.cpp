// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Address.h"
#include "../Base58.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include <cstring>

namespace TW::Nebulas {

bool Address::isValid(const std::string& string) {
    auto data = Base58::decode(string);
    if (data.size() != (size_t)Address::size) {
        return false;
    }

    if (data[0] != Address::AddressPrefix) {
        return false;
    }
    if (data[1] != Address::NormalType && data[1] != Address::ContractType) {
        return false;
    }

    Data content(data.begin(), data.begin() + 22);
    Data checksum(data.begin() + 22, data.end());
    auto dataSha3 = Hash::sha3_256(content);
    return std::memcmp(dataSha3.data(), checksum.data(), 4) == 0;
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }

    auto data = Base58::decode(string);
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const Data& data) {
    if (!Base58Address::isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1Extended) {
        throw std::invalid_argument("Nebulas::Address needs an extended SECP256k1 public key.");
    }
    const auto data = publicKey.hash(
        {Address::AddressPrefix, Address::NormalType},
        Hash::HasherSha3_256ripemd, false);

    std::copy(data.begin(), data.end(), bytes.begin());
    auto checksum = Hash::sha3_256(data);
    std::copy(checksum.begin(), checksum.begin() + 4, bytes.begin() + 22);
}

std::string Address::string() const {
    return Base58::encode(bytes);
}

} // namespace TW::Nebulas
