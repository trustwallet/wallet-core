// Copyright © 2017-2023 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace TW::Filecoin {

class Address {
  public:
    enum class Type : uint8_t {
        ID = 0,
        SECP256K1 = 1,
        ACTOR = 2,
        BLS = 3,
        DELEGATED = 4,
        Invalid,
    };

    /// Type of the Address.
    Type type = Type::Invalid;

    /// Actor ID.
    /// This is used if `type` is either `ID` or `DELEGATED`.
    uint64_t actorID = 0;

    /// Address data payload *only*.
    Data bytes;

    /// Determines whether a string makes a valid encoded address.
    static bool isValid(const std::string& string);

    /// Initializes an address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes an address with a collection of bytes.
    explicit Address(const Data& data);

    /// Initializes an address with a secp256k1 public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    [[nodiscard]] std::string string() const;

    /// Returns the type of an address.
    Type type() const { return getType(bytes[0]); }

    /// Address prefix
    static constexpr char PREFIX = 'f';

  public:
    /// Attempts to get the type by number.
    static Type getType(uint8_t raw) {
        switch (raw) {
        case 0:
            return Type::ID;
        case 1:
            return Type::SECP256K1;
        case 2:
            return Type::ACTOR;
        case 3:
            return Type::BLS;
        case 4:
            return Type::DELEGATED;
        default:
            return Type::Invalid;
        }
    }

    /// Attempts to get the type by ASCII.
    static Type parseType(char c) {
        if (c >= '0' && c <= '4') {
            return static_cast<Type>(c - '0');
        } else {
            return Type::Invalid;
        }
    }

    /// Returns ASCII character of type
    static char typeAscii(Type t) { return '0' + static_cast<char>(t); }

    // Returns the payload size (excluding any prefixes) of an address type.
    // If the payload size is undefined/variable (e.g. ID)
    // or the type is unknown, it returns zero.
    static uint8_t payloadSize(Type t) {
        switch (t) {
        case Type::SECP256K1:
        case Type::ACTOR:
        case Type::DELEGATED:
            return 20;
        case Type::BLS:
            return 48;
        default:
            return 0;
        }
    }

    /// Validates if the payload size (excluding any prefixes) of an address type has an expected value.
    static bool isValidPayloadSize(Type t, std::size_t payloadSize) {
        switch (t) {
            case Type::ID:
                return payloadSize == 0;
            case Type::SECP256K1:
            case Type::ACTOR:
                return payloadSize == 20;
            case Type::DELEGATED:
                return payloadSize <= 54;
            default:
                return false;
        }
    }
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::Filecoin
