// Copyright © 2017-2023 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include <cstring>
#include <climits>
#include <optional>

#include "../Base32.h"

namespace TW::Filecoin {

static const char BASE32_ALPHABET_FILECOIN[] = "abcdefghijklmnopqrstuvwxyz234567";
static constexpr size_t checksumSize = 4;

/// Decodes the given `string` as an ActorID.
/// Please note `string` must not contain any prefixes.
static bool decodeActorID(const std::string& string, uint64_t& actorID) {
    if (string.length() > 20) {
        return false;
    }
    try {
        size_t chars;
        actorID = std::stoull(string, &chars);
        return chars > 0;
    } catch (...) {
        return false;
    }
}

static void writeActorID(uint64_t actorID, Data& dest) {
    while (actorID >= 0x80) {
        dest.push_back(((uint8_t)actorID) | 0x80);
        actorID >>= 7;
    }
    dest.push_back((uint8_t)actorID);
}

/// Returns encoded bytes of Address including the protocol byte and actorID (if required)
/// without the checksum.
static Data&& addressToBytes(Address::Type type, uint64_t actorID, const Data& payload) {
    Data encoded;
    encoded.push_back(static_cast<uint8_t>(type));
    if (type == Address::Type::ID || type == Address::Type::DELEGATED) {
        writeActorID(actorID, encoded);
    }
    encoded.insert(std::end(encoded), std::begin(payload), std::end(payload));
    return std::move(encoded);
}

static Data&& calculateChecksum(Address::Type type, uint64_t actorID, const Data& payload) {
    Data bytesVec(addressToBytes(type, actorID, payload));
    Data sum(Hash::blake2b(bytesVec, checksumSize));
    assert(sum.size() == checksumSize);
    return std::move(sum);
}

/// Decodes `string` as a Filecoin address and validates the checksum.
/// Returns `true` if `string` is valid address.
static bool decodeValidateAddress(const std::string& string, Address::Type& type, uint64_t& actorID, Data& payload) {
    if (string.length() < 3) {
        return false;
    }
    // Only main net addresses supported.
    if (string[0] != Address::PREFIX) {
        return false;
    }

    // Get address type.
    type = Address::parseType(string[1]);
    if (type == Address::Type::Invalid) {
        return false;
    }

    if (type == Address::Type::ID) {
        return decodeActorID(string.substr(2), actorID);
    }

    // For `SECP256K1`, `ACTOR`, `BLS`, the payload starts after the Protocol Type.
    // For `DELEGATED`, the payload starts after the ActorID and 'f' separator.
    std::size_t payloadPos = 2;
    if (type == Address::Type::DELEGATED) {
        std::size_t actorIDEnd = string.find('f');
        // Delegated address must contain 'f' separator.
        if (actorIDEnd == std::string::npos || actorIDEnd <= 2) {
            return false;
        }
        if (!decodeActorID(string.substr(2, actorIDEnd - 2), actorID)) {
            return false;
        }
        // Address payload starts after 'f'.
        payloadPos = actorIDEnd + 1;
    }

    Data decoded;
    if (!Base32::decode(string.substr(payloadPos), decoded, BASE32_ALPHABET_FILECOIN)) {
        return false;
    }
    if (decoded.size() < checksumSize) {
        return false;
    }
    payload.assign(decoded.begin(), decoded.end() - checksumSize);
    if (!Address::isValidPayloadSize(type, payload.size())) {
        return false;
    }

    Data actualChecksum(decoded.end() - checksumSize, decoded.end());
    Data expectedChecksum = calculateChecksum(type, actorID, payload);
    return actualChecksum == expectedChecksum;
}

bool Address::isValid(const std::string& string) {
    Type type;
    uint64_t actorID;
    Data payload;
    return decodeValidateAddress(string, type, actorID, payload);
}

Address::Address(const std::string& string) {
    if (!decodeValidateAddress(string, type, actorID, payload)) {
        throw std::invalid_argument("Invalid address data");
    }
}

Address::Address(const PublicKey &publicKey):
    type(Type::SECP256K1),
    actorID(0),
    payload(Hash::blake2b(publicKey.bytes, 20)) {
}

Data&& Address::toBytes() const {
    return std::move(addressToBytes(type, actorID, payload));
}

std::string Address::string() const {
    std::string s;
    // Main net address prefix
    s.push_back(PREFIX);
    // Address type prefix
    s.push_back(typeAscii(type));

    if (type == Type::ID) {
        s.append(std::to_string(actorID));
        return s;
    }

    Data toEncode(payload.begin(), payload.end());

    // Append Blake2b checksum
    Data checksum = calculateChecksum(type, actorID, payload);
    toEncode.insert(toEncode.end(), checksum.begin(), checksum.end());

    s.append(Base32::encode(toEncode, BASE32_ALPHABET_FILECOIN));
    return s;
}

} // namespace TW::Filecoin
