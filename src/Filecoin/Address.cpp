// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Address.h"

#include <algorithm>
#include <climits>

#include "../Base32.h"

namespace TW::Filecoin {

static const char BASE32_ALPHABET_FILECOIN[] = "abcdefghijklmnopqrstuvwxyz234567";
static constexpr std::size_t checksumSize = 4;

static constexpr uint64_t charMask = 0x80;

/// Parses the given `string` as an ActorID.
/// Please note `string` must not contain any prefixes.
static bool parseActorID(const std::string& string, uint64_t& actorID) {
    // `uint64_t` may contain up to 20 decimal digits.
    static constexpr std::size_t maxDigits = 20;

    if (string.length() > maxDigits) {
        return false;
    }
    const bool onlyDigits = std::all_of(
        string.begin(),
        string.end(),
        [](unsigned char c)->bool { return std::isdigit(c); }
    );
    if (!onlyDigits) {
        return false;
    }

    try {
        std::size_t chars;
        actorID = std::stoull(string, &chars);
        return chars > 0;
    } catch (...) {
        return false;
    }
}

/// Decodes the given `string` as an ActorID.
/// Please note `bytes` must not contain any prefixes.
/// https://github.com/paritytech/unsigned-varint/blob/a3a5b8f2bee1f44270629e96541adf805a53d32c/src/decode.rs#L66-L86
static bool decodeActorID(const Data& bytes, uint64_t& actorID, std::size_t& remainingPos) {
    static constexpr std::size_t maxBytes = 9;

    actorID = 0;
    remainingPos = 0;
    for (remainingPos = 0; remainingPos < bytes.size() && remainingPos <= maxBytes; ++remainingPos) {
        auto byte = bytes[remainingPos];
        auto k = byte & SCHAR_MAX;
        actorID |= k << (remainingPos * 7);

        // Check if last.
        if ((byte & charMask) == 0) {
            if (byte == 0 && remainingPos > 0) {
                // If last byte is zero, it could have been "more minimally" encoded by dropping that trailing zero.
                return false;
            }
            ++remainingPos;
            return true;
        }
    }

    // Couldn't find the last byte so `(byte & 0x80) == 0`.
    return false;
}

static void writeActorID(uint64_t actorID, Data& dest) {
    static constexpr uint64_t shift = 7;

    while (actorID >= charMask) {
        dest.emplace_back(actorID | charMask);
        actorID >>= shift;
    }
    dest.emplace_back(actorID);
}

/// Returns encoded bytes of Address including the protocol byte and actorID (if required)
/// without the checksum.
static Data addressToBytes(Address::Type type, uint64_t actorID, const Data& payload) {
    Data encoded;
    encoded.push_back(static_cast<uint8_t>(type));
    if (type == Address::Type::ID || type == Address::Type::DELEGATED) {
        writeActorID(actorID, encoded);
    }
    append(encoded, payload);
    return encoded;
}

static Data calculateChecksum(Address::Type type, uint64_t actorID, const Data& payload) {
    Data bytesVec(addressToBytes(type, actorID, payload));
    Data sum = Hash::blake2b(bytesVec, checksumSize);
    assert(sum.size() == checksumSize);
    return sum;
}

MaybeAddress Address::fromBytes(const Data& encoded) {
    // Should contain at least one byte (address type).
    if (encoded.empty()) {
        return std::nullopt;
    }

    // Get address type.
    Type type = Address::getType(encoded[0]);
    Data withoutPrefix(encoded.begin() + 1, encoded.end());

    switch (type) {
        case Address::Type::ID: {
            std::size_t remainingPos = 0;
            uint64_t actorID = 0;

            if (!decodeActorID(withoutPrefix, actorID, remainingPos)) {
                return std::nullopt;
            }
            // Check if there are no remaining bytes.
            if (remainingPos != withoutPrefix.size()) {
                return std::nullopt;
            }

            return Address(type, actorID, Data());
        }
        case Address::Type::SECP256K1:
        case Address::Type::ACTOR:
        case Address::Type::BLS: {
            if (!Address::isValidPayloadSize(type, withoutPrefix.size())) {
                return std::nullopt;
            }
            return Address(type, 0, std::move(withoutPrefix));
        }
        case Address::Type::DELEGATED: {
            std::size_t remainingPos = 0;
            uint64_t actorID = 0;

            if (!decodeActorID(withoutPrefix, actorID, remainingPos)) {
                return std::nullopt;
            }
            if (!Address::isValidPayloadSize(type, withoutPrefix.size() - remainingPos)) {
                return std::nullopt;
            }

            return Address(type, actorID, subData(withoutPrefix, remainingPos));
        }
        default:
            return std::nullopt;
    }
}

MaybeAddress Address::fromString(const std::string& string) {
    // An address must contain at least 'f' prefix and the address type.
    static constexpr std::size_t minLength = 2;

    if (string.length() < minLength) {
        return std::nullopt;
    }
    // Only main net addresses supported.
    if (string[0] != Address::PREFIX) {
        return std::nullopt;
    }

    // Get address type.
    Type type = Address::parseType(string[1]);
    if (type == Address::Type::Invalid) {
        return std::nullopt;
    }

    uint64_t actorID = 0;
    if (type == Address::Type::ID) {
        if (!parseActorID(string.substr(2), actorID)) {
            return std::nullopt;
        }
        return Address(type, actorID, Data());
    }

    // For `SECP256K1`, `ACTOR`, `BLS`, the payload starts after the Protocol Type.
    // For `DELEGATED`, the payload starts after an ActorID and the 'f' separator.
    std::size_t payloadPos = 2;
    if (type == Address::Type::DELEGATED) {
        std::size_t actorIDEnd = string.find('f', 2);
        // Delegated address must contain the 'f' separator.
        if (actorIDEnd == std::string::npos || actorIDEnd <= 2) {
            return std::nullopt;
        }
        if (!parseActorID(string.substr(2, actorIDEnd - 2), actorID)) {
            return std::nullopt;
        }
        // Address payload starts after 'f'.
        payloadPos = actorIDEnd + 1;
    }

    Data decoded;
    if (!Base32::decode(string.substr(payloadPos), decoded, BASE32_ALPHABET_FILECOIN)) {
        return std::nullopt;
    }
    if (decoded.size() < checksumSize) {
        return std::nullopt;
    }

    Data payload(decoded.begin(), decoded.end() - checksumSize);
    if (!Address::isValidPayloadSize(type, payload.size())) {
        return std::nullopt;
    }

    Data actualChecksum(decoded.end() - checksumSize, decoded.end());
    Data expectedChecksum = calculateChecksum(type, actorID, payload);
    if (actualChecksum != expectedChecksum) {
       return std::nullopt;
    }

    return Address(type, actorID, std::move(payload));
}

bool Address::isValid(const std::string& string) {
    return Address::fromString(string).has_value();
}

bool Address::isValid(const Data& encoded) {
    return Address::fromBytes(encoded).has_value();
}

Address Address::secp256k1Address(const PublicKey& publicKey) {
    Data payload = Hash::blake2b(publicKey.bytes, 20);
    return Address(Type::SECP256K1, 0, std::move(payload));
}

Address Address::delegatedAddress(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1Extended) {
        throw std::invalid_argument("Ethereum::Address needs an extended SECP256k1 public key.");
    }

    const auto data = publicKey.hash({}, Hash::HasherKeccak256, true);
    Data payload(data.end() - 20, data.end());

    return Address(Type::DELEGATED, ETHEREUM_ADDRESS_MANAGER_ACTOR_ID, std::move(payload));
}

Address Address::delegatedAddress(uint64_t actorID, Data&& payload) {
    return Address(Type::DELEGATED, actorID, std::move(payload));
}

Address::Address(const std::string& string) {
    auto addr = Address::fromString(string);
    if (!addr) {
        throw std::invalid_argument("Invalid address data");
    }

    assign(std::move(*addr));
}

Address::Address(const Data& encoded) {
    auto addr = Address::fromBytes(encoded);
    if (!addr) {
        throw std::invalid_argument("Invalid address data");
    }

    assign(std::move(*addr));
}

Address::Address(Type type, uint64_t actorID, Data&& payload):
    type(type),
    actorID(actorID),
    payload(std::move(payload)) {
    if (!isValidPayloadSize(this->type, this->payload.size())) {
        throw std::invalid_argument("Invalid address data");
    }
}

void Address::assign(Address&& other) {
    type = other.type;
    actorID = other.actorID;
    payload = std::move(other.payload);
}

Data Address::toBytes() const {
    return addressToBytes(type, actorID, payload);
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

    if (type == Type::DELEGATED) {
        s.append(std::to_string(actorID));
        s.push_back('f');
    }

    // Append Blake2b checksum
    Data checksum = calculateChecksum(type, actorID, payload);

    Data toEncode = payload;
    append(toEncode, checksum);

    s.append(Base32::encode(toEncode, BASE32_ALPHABET_FILECOIN));
    return s;
}

} // namespace TW::Filecoin
