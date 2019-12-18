// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Base32.h"
#include "../Data.h"

using namespace TW;
using namespace TW::Filecoin;

static const char BASE32_ALPHABET_FILECOIN[] = "abcdefghijklmnopqrstuvwxyz234567";
static constexpr size_t checksum_size = 4;

bool Address::isValid(const Data& data) {
    if (data.size() < 2) {
        return false;
    }
    if (!get_type(data[0])) {
        return false;
    }
    Type type = *get_type(data[0]);
    if (type == Type::ID) {
        // Verify varuint encoding
        if (data.size() > 11)
            return false;
        if (data.size() == 11 && data[10] > 0x01)
            return false;
        int i;
        for (i = 1; i < data.size(); i++)
            if ((data[i] & 0x80) == 0)
                break;
        return i == data.size() - 1;
    } else {
        return data.size() == 1 + Address::payload_size(type);
    }
}

static bool isValidID(const std::string& string) {
    if (string.length() > 22)
        return false;
    for (int i = 2; i < string.length(); i++) {
        if (string[i] < '0' || string[i] > '9') {
            return false;
        }
    }
    try {
        size_t chars;
        std::stoull(string.substr(2), &chars);
        return chars > 0;
    } catch (...) {
        return false;
    }
}

static bool isValidBase32(const std::string& string, Address::Type type) {
    // Check if valid Base32.
    uint8_t size = Address::payload_size(type);
    Data decoded;
    if (!Base32::decode(string.substr(2), decoded, BASE32_ALPHABET_FILECOIN)) {
        return false;
    }

    // Check size
    if (decoded.size() != size + 4) {
        return false;
    }

    // Extract raw address.
    Data address;
    address.push_back(static_cast<uint8_t>(type));
    address.insert(address.end(), decoded.data(), decoded.data() + size);

    // Verify checksum.
    Data should_sum = Hash::blake2b(address, checksum_size);
    return std::memcmp(should_sum.data(), decoded.data() + size, checksum_size) == 0;
}

bool Address::isValid(const std::string& string) {
    if (string.length() < 3) {
        return false;
    }
    // Only main net addresses supported.
    if (string[0] != PREFIX) {
        return false;
    }
    // Get address type.
    auto type = parse_type(string[1]);
    if (!type) {
        return false;
    }

    // ID addresses are special, they are just numbers.
    return type == Type::ID ? isValidID(string) : isValidBase32(string, *type);
}

Address::Address(const std::string& string) {
    if (!isValid(string))
        throw std::invalid_argument("Invalid address data");

    Type type = *(parse_type(string[1]));
    // First byte is type
    bytes.push_back(static_cast<uint8_t>(type));
    if (type == Type::ID) {
        uint64_t id = std::stoull(string.substr(2));
        while (id >= 0x80) {
            bytes.push_back(((uint8_t)id) | 0x80);
            id >>= 7;
        }
        bytes.push_back((uint8_t)id);
        return;
    }

    Data decoded;
    if (!Base32::decode(string.substr(2), decoded, BASE32_ALPHABET_FILECOIN))
        throw std::invalid_argument("Invalid address data");
    uint8_t payload_size = Address::payload_size(type);

    bytes.insert(bytes.end(), decoded.data(), decoded.data() + payload_size);
}

Address::Address(const Data& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    bytes = data;
}

Address::Address(const PublicKey& publicKey) {
    bytes.push_back(static_cast<uint8_t>(Type::SECP256K1));
    Data hash = Hash::blake2b(publicKey.bytes, payload_size(Type::SECP256K1));
    bytes.insert(bytes.end(), hash.begin(), hash.end());
}

std::string Address::string() const {
    std::string s;
    // Main net address prefix
    s.push_back(PREFIX);
    // Address type prefix
    s.push_back(type_ascii(type()));

    if (type() == Type::ID) {
        uint64_t id = 0;
        unsigned shift = 0;
        for (int i = 1; i < bytes.size(); i++) {
            if (bytes[i] < 0x80) {
                id |= bytes[i] << shift;
                break;
            } else {
                id |= ((uint64_t)(bytes[i] & 0x7F)) << shift;
                shift += 7;
            }
        }
        s.append(std::to_string(id));
        return s;
    }

    uint8_t payload_size = Address::payload_size(type());
    // Base32 encoded body
    Data to_encode(payload_size + checksum_size);
    // Copy address payload without prefix
    std::copy(bytes.data() + 1, bytes.data() + payload_size + 1, to_encode.data());
    // Append Blake2b checksum
    Data bytes_vec;
    bytes_vec.assign(std::begin(bytes), std::end(bytes));
    Data sum = Hash::blake2b(bytes_vec, checksum_size);
    assert(sum.size() == checksum_size);
    std::copy(sum.begin(), sum.end(), to_encode.data() + payload_size);
    s.append(Base32::encode(to_encode, BASE32_ALPHABET_FILECOIN));

    return s;
}
