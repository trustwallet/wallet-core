// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>

#include "../PublicKey.h"
#include "../Data.h"

namespace TW {
namespace EOS {

class Address {
public:
    enum class Type {
        Legacy,
        ModernK1,
        ModernR1,
    };

    /// Determines whether a string makes a valid EOS address.
    static bool isValid(const std::string& string);

    /// Determines whether the given byte array is a valid keyBuffer
    static bool isValid(const uint8_t *buffer, size_t size, Type type = Type::Legacy);

    /// Initializes a EOS address from a string representation.
    Address(const std::string& string);

    /// Initializes a EOS address from a public key.
    Address(const PublicKey& publicKey, Type type = Type::Legacy);

    /// Initializes a EOS address from a byte array.
    Address(const Data& data, Type type = Type::Legacy);

    /// Returns a string representation of the EOS address.
    std::string string() const;

    friend bool operator==(const Address& lhs, const Address& rhs);

    inline const std::string prefix() { return prefixForType(type); }

    static std::string prefixForType(Type t) {
        switch (t) {
        case Type::Legacy:
            return Legacy::prefix;

        case Type::ModernK1:
            return Modern::K1::fullPrefix;

        case Type::ModernR1:
            return Modern::R1::fullPrefix;
        }
    }

protected:
    // Class constants
    static const size_t PublicKeyDataSize = 33;
    static const size_t ChecksumSize = 4;
    static const size_t KeyDataSize = PublicKeyDataSize + ChecksumSize;
    static const size_t Base58Size = KeyDataSize * 138 / 100 + 2; // a buffer size large enough to fit base58 representation of the key data

    struct Legacy {
        static constexpr const char *prefix = "EOS";
        static const size_t prefixSize = 3;
    };

    struct Modern {
        static constexpr const char *basePrefix = "PUB";
        static const size_t basePrefixSize = 3;

        struct K1 {
            static constexpr const char *subPrefix = "K1";
            static const size_t subPrefixSize = 2;
            static constexpr const char *fullPrefix = "PUB_K1_";
            static const size_t fullPrefixSize = 7;
        };

        struct R1 {
            static constexpr const char *subPrefix = "R1";
            static const size_t subPrefixSize = 2;
            static constexpr const char *fullPrefix = "PUB_R1_";
            static const size_t fullPrefixSize = 7;
        };
    };


    uint8_t keyData[KeyDataSize];
    Type type;

    static uint32_t createChecksum(const uint8_t *buffer, Address::Type type);
    static bool extractKeyData(const std::string& string, Address *address = nullptr);
};


inline bool operator==(const Address& lhs, const Address& rhs) {
    return memcmp(lhs.keyData, rhs.keyData, Address::KeyDataSize) == 0
             && lhs.type == rhs.type;
}

}} // namespace

struct TWEOSAddress {
    TW::EOS::Address impl;
};