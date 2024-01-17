// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../PublicKey.h"

#include <array>
#include <cstdint>
#include <optional>
#include <vector>
#include <string>

namespace TW::Filecoin {

class Address;

using MaybeAddress = std::optional<Address>;

class Address {
  public:
    /// The actor ID of the Ethereum Address Manager singleton.
    static constexpr uint64_t ETHEREUM_ADDRESS_MANAGER_ACTOR_ID = 10;

    enum class Type : uint8_t {
        ID = 0,
        SECP256K1 = 1,
        ACTOR = 2,
        BLS = 3,
        DELEGATED = 4,
        Invalid,
    };

    /// Type of the Address.
    Type type{Type::Invalid};

    /// Actor ID.
    /// This is used if `type` is either `ID` or `DELEGATED`.
    uint64_t actorID{0};

    /// Address data payload (without prefixes and checksum).
    Data payload;

    /// Decodes `encoded` as a Filecoin address.
    /// Returns `std::nullopt` on fail.
    static MaybeAddress fromBytes(const Data& encoded);

    /// Parses `string` as a Filecoin address and validates the checksum.
    /// Returns `std::nullopt` if `string` is not a valid address.
    static MaybeAddress fromString(const std::string& string);

    /// Determines whether a string makes a valid encoded address.
    static bool isValid(const std::string& string);

    /// Determines whether a collection of bytes makes a valid address.
    static bool isValid(const Data& encoded);

    /// Initializes a Secp256k1 address with a secp256k1 public key.
    static Address secp256k1Address(const PublicKey& publicKey);

    /// Initializes a Delegated address with a secp256k1 public key.
    static Address delegatedAddress(const PublicKey& publicKey);

    /// Initializes a Delegated address with a secp256k1 public key.
    static Address delegatedAddress(uint64_t actorID, Data&& payload);

    /// Initializes an address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes an address with a collection of bytes.
    explicit Address(const Data& encoded);

    /// Returns a string representation of the address.
    [[nodiscard]] std::string string() const;

    /// Returns encoded bytes of Address including the protocol byte and actorID (if required)
    /// without the checksum.
    Data toBytes() const;

    /// Address prefix
    static constexpr char PREFIX = 'f';

private:
    static constexpr char F0_TYPE_CHAR = '0';
    static constexpr char F4_TYPE_CHAR = '4';

    /// Initializes an address with a type, actorID and payload.
    explicit Address(Type type, uint64_t actorID, Data&& payload);

    /// Assigns the address to the `other` value.
    void assign(Address&& other);

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
        if (c >= F0_TYPE_CHAR && c <= F4_TYPE_CHAR) {
            return static_cast<Type>(c - F0_TYPE_CHAR);
        } else {
            return Type::Invalid;
        }
    }

    /// Returns ASCII character of type
    static char typeAscii(Type t) { return F0_TYPE_CHAR + static_cast<char>(t); }

    /// Validates if the payload size (excluding any prefixes and checksum) of an address type has an expected value.
    static bool isValidPayloadSize(Type t, std::size_t payloadSize) {
        switch (t) {
            case Type::ID:
                return payloadSize == 0;
            case Type::SECP256K1:
            case Type::ACTOR:
                return payloadSize == 20;
            case Type::BLS:
                return payloadSize == 48;
            case Type::DELEGATED:
                return payloadSize <= 54;
            default:
                return false;
        }
    }
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.type == rhs.type
           && lhs.actorID == rhs.actorID
           && lhs.payload == rhs.payload;
}

} // namespace TW::Filecoin
