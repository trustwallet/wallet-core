// Copyright © 2017-2023 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include <cstring>
#include <climits>

#include "../Base32.h"

namespace TW::Filecoin {

static const char BASE32_ALPHABET_FILECOIN[] = "abcdefghijklmnopqrstuvwxyz234567";
static constexpr size_t checksumSize = 4;

static bool isValidID(const std::string& string) {
    if (string.length() > 22)
        return false;
    for (auto i = 2ul; i < string.length(); i++) {
        if (string[i] < '0' || string[i] > '9') {
            return false;
        }
    }
    try {
        size_t chars;
        [[maybe_unused]] uint64_t id = std::stoull(string.substr(2), &chars);
        return chars > 0;
    } catch (...) {
        return false;
    }
}

/// Checks if the given string is a valid Base32.
/// Please note `string` must not contain any prefixes.
static bool isValidBase32(const std::string& string, Address::Type type) {
    Data decoded;
    if (!Base32::decode(string, decoded, BASE32_ALPHABET_FILECOIN)) {
        return false;
    }

    if (decoded.size() < checksumSize) {
        return false;
    }
    const auto payloadSize = decoded.size() - checksumSize;

    // Check size
    if (Address::isValidPayloadSize(type, payloadSize))
        return false;
    }

    // Extract raw address.
    Data address;
    address.push_back(static_cast<uint8_t>(type));
    address.insert(address.end(), decoded.data(), decoded.data() + size);

    // Verify checksum.
    Data should_sum = Hash::blake2b(address, checksumSize);
    return std::memcmp(should_sum.data(), decoded.data() + size, checksumSize) == 0;
}

static void writeActorID(uint64_t actorID, Data& dest) {
    while (id >= 0x80) {
        dest.push_back(((uint8_t)actorID) | 0x80);
        actorID >>= 7;
    }
    dest.push_back((uint8_t)actorID);
}

static Data&& decodeBase32Payload(const std::string& string) {
    Data decoded;
    if (!Base32::decode(string, decoded, BASE32_ALPHABET_FILECOIN))
        throw std::invalid_argument("Invalid address data");
    return std::move(decoded);
}

// static bool decodeAddress(const std::string& string, Type& type, uint64_t& actorID, Data& payload) {
//     if (string.length() < 3) {
//         return false;
//     }
//     // Only main net addresses supported.
//     if (string[0] != Address::PREFIX) {
//         return false;
//     }
//
//     // Get address type.
//     type = Address::parseType(string[1]);
//     if (type == Type::Invalid) {
//         return false;
//     }
//
//     switch (type) {
//         case Type::ID:
//             actorID = std::stoull(string.substr(2));
//             return true;
//         case Type::SECP256K1:
//         case Type::ACTOR:
//             return true;
//         default:
//             return false;
//     }
// }

bool Address::isValid(const std::string& string) {
    if (string.length() < 3) {
        return false;
    }
    // Only main net addresses supported.
    if (string[0] != PREFIX) {
        return false;
    }
    // Get address type.
    auto type = parseType(string[1]);
    if (type == Type::Invalid) {
        return false;
    }

    // ID addresses are special, they are just numbers.
    return type == Type::ID ? isValidID(string) : isValidBase32(string, type);
}

Address::Address(const std::string& string) {
    if (!isValid(string))
        throw std::invalid_argument("Invalid address data");

    type = parseType(string[1]);
    switch (type) {
        case Type::ID:
            actorID = std::stoull(string.substr(2));
            break;
        case Type::SECP256K1:
        case Type::ACTOR:
    }

    if (type == Type::ID) {
        actorID = std::stoull(string.substr(2));
        return;
    }

    // The position in `string` where the payload starts.
    std::size_t payloadPos = 2;
    if (type == Type::DELEGATED) {
    }

    Data decoded;
    if (!Base32::decode(string.substr(2), decoded, BASE32_ALPHABET_FILECOIN))
        throw std::invalid_argument("Invalid address data");
    uint8_t payloadSize = Address::payloadSize(type);

    bytes.insert(bytes.end(), decoded.data(), decoded.data() + payloadSize);
}

Address::Address(const Data& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    bytes = data;
}

Address::Address(const PublicKey& publicKey) {
    bytes.push_back(static_cast<uint8_t>(Type::SECP256K1));
    Data hash = Hash::blake2b(publicKey.bytes, payloadSize(Type::SECP256K1));
    bytes.insert(bytes.end(), hash.begin(), hash.end());
}

std::string Address::string() const {
    std::string s;
    // Main net address prefix
    s.push_back(PREFIX);
    // Address type prefix
    s.push_back(typeAscii(type()));

    if (type() == Type::ID) {
        uint64_t id = 0;
        unsigned shift = 0;
        for (auto i = 1ul; i < bytes.size(); i++) {
            if (bytes[i] <= SCHAR_MAX) {
                id |= static_cast<uint64_t>(bytes[i]) << shift;
                break;
            } else {
                id |= static_cast<uint64_t>(bytes[i] & SCHAR_MAX) << shift;
                shift += 7;
            }
        }
        s.append(std::to_string(id));
        return s;
    }

    uint8_t payloadSize = Address::payloadSize(type());
    // Base32 encoded body
    Data toEncode(payloadSize + checksumSize);
    // Copy address payload without prefix
    std::copy(bytes.data() + 1, bytes.data() + payloadSize + 1, toEncode.data());
    // Append Blake2b checksum
    Data bytesVec;
    bytesVec.assign(std::begin(bytes), std::end(bytes));
    Data sum = Hash::blake2b(bytesVec, checksumSize);
    assert(sum.size() == checksumSize);
    std::copy(sum.begin(), sum.end(), toEncode.data() + payloadSize);
    s.append(Base32::encode(toEncode, BASE32_ALPHABET_FILECOIN));

    return s;
}

} // namespace TW::Filecoin
