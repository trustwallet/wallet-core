// Copyright © 2019 Mart Roosmaa.
// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Address.h"
#include "../Hash.h"
#include "../Base32.h"

#include <string>

namespace TW::Nano {

static const char* BASE32_ALPHABET_NANO = "13456789abcdefghijkmnopqrstuwxyz";

const size_t CHECKSUM_LEN = 5;
const size_t PADDING_LEN = 3;
const size_t ADDRESS_BASE_LENGTH = 60;
const size_t PUBLIC_KEY_LEN = 32;
const size_t RAW_LEN = PADDING_LEN + PUBLIC_KEY_LEN + CHECKSUM_LEN;

const std::string kPrefixNano{"nano_"};
const std::string kPrefixXrb{"xrb_"};

bool isValidInternal(const std::string& address, const std::string& prefix, uint8_t* publicKey) {
    if (address.length() != prefix.length() + ADDRESS_BASE_LENGTH) {
        return false;
    }

    // Validate that the prefix matches
    if (address.substr(0, prefix.length()) != prefix) {
        return false;
    }

    // Try to decode the address
    std::string encoded = "1111" + address.substr(prefix.length());
    
    Data raw;
    raw.resize(RAW_LEN);
    if (!Base32::decode(encoded, raw, BASE32_ALPHABET_NANO)) {
        return false;
    }

    // Validate the checksum
    Data checksum = Hash::blake2b(
        Data(raw.begin() + PADDING_LEN, raw.begin() + PADDING_LEN + PUBLIC_KEY_LEN),
        CHECKSUM_LEN
    );

    // Compare checksums
    for (size_t i = 0; i < CHECKSUM_LEN; i++) {
        if (raw[40 - CHECKSUM_LEN + i] != checksum[CHECKSUM_LEN - (i + 1)]) {
            return false;
        }
    }

    if (publicKey != nullptr) {
        std::copy(raw.begin() + PADDING_LEN, raw.begin() + PADDING_LEN + 32, publicKey);
    }

    return true;
}

bool Address::isValid(const std::string& address) {
    bool valid = false;

    valid = isValidInternal(address, kPrefixNano, nullptr);
    if (!valid) {
        valid = isValidInternal(address, kPrefixXrb, nullptr);
    }

    return valid;
}

Address::Address(const std::string& address) {
    bool valid = false;

    valid = isValidInternal(address, kPrefixNano, bytes.data());

    if (!valid) {
        valid = isValidInternal(address, kPrefixXrb, bytes.data());
    }

    // Ensure address is valid
    if (!valid) {
        throw std::invalid_argument("Invalid address data");
    }
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeED25519Blake2b) {
        throw std::invalid_argument("Invalid public key type");
    }

    auto keyBytes = publicKey.bytes;
    std::copy(keyBytes.begin(), keyBytes.begin() + 32, bytes.data());
}

std::string Address::string() const {
    Data bytesAsData;
    bytesAsData.assign(bytes.begin(), bytes.end());
    auto hash = Hash::blake2b(bytesAsData, CHECKSUM_LEN);
    bytesAsData.insert(bytesAsData.begin(), PADDING_LEN, 0);
    bytesAsData.insert(bytesAsData.end(), hash.rbegin(), hash.rend());
    
    auto result = Base32::encode(bytesAsData, BASE32_ALPHABET_NANO);
    result.replace(0, kPrefixNano.length()-1, kPrefixNano);
    return result;
}

} // namespace TW::Nano
