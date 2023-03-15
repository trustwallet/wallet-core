// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Base58.h"
#include "Data.h"
#include "../Hash.h"

#include <HexCoding.h>
#include <cassert>
#include <cstring>
#include <stdexcept>

namespace TW::Waves {

template <typename T>
Data Address::secureHash(const T &data) {
    return Hash::keccak256(Hash::blake2b(data, 32));
}

bool Address::isValid(const Data& decoded) {
    if (decoded.size() != Address::size) {
        return false;
    }

    if (decoded[0] != v1) {
        return false;
    }

    if (decoded[1] != mainnet) {
        return false;
    }

    const auto data = Data(decoded.begin(), decoded.end() - 4);
    const auto data_checksum = Data(decoded.end() - 4, decoded.end());
    const auto calculated_hash = secureHash(data);
    const auto calculated_checksum = Data(calculated_hash.begin(), calculated_hash.begin() + 4);
    return std::memcmp(data_checksum.data(), calculated_checksum.data(), 4) == 0;
}

bool Address::isValid(const std::string& string) {
    const auto decoded = Base58::decode(string);
    return isValid(decoded);
}

Address::Address(const std::string& string) {
    const auto decoded = Base58::decode(string);
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address key data");
    }
    std::copy(decoded.begin(), decoded.end(), bytes.begin());
}

Address::Address(const Data& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey &publicKey) {
    if (publicKey.type != TWPublicKeyTypeCURVE25519) {
        throw std::invalid_argument("Invalid public key type");
    }
    const auto pkdata = Data(publicKey.bytes.begin(), publicKey.bytes.end());
    const auto keyhash = Address::secureHash(pkdata);
    bytes[0] = v1;
    bytes[1] = mainnet;
    std::copy(keyhash.begin(), keyhash.begin() + 20, bytes.begin() + 2);

    const auto checksum_data = Data(bytes.begin(), bytes.begin() + 22);
    const auto checksum = Hash::keccak256(Hash::blake2b(checksum_data, 32));

    std::copy(checksum.begin(), checksum.begin() + 4, bytes.begin() + 22);
}

std::string Address::string() const {
    return Base58::encode(bytes);
}

} // namespace TW::Waves
