// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW {
namespace Stellar {

class Address {
public:

    // 56 character base-32 encoded string
    static const size_t STELLAR_ADDRESS_SIZE = 56;
    // Decodes to 35 bytes
    static const size_t STELLAR_ADDRESS_SIZE_RAW = 35;
    // Raw key size is 32 bytes
    static const size_t STELLAR_KEY_SIZE = 32;

    /// Address data consisting of a prefix byte followed by the public key hash.
    std::array<byte, STELLAR_KEY_SIZE> bytes;

    /// Determines whether a collection of bytes makes a valid Stellar address.
    template<typename T>
    static bool isValid(const T& data) {
        return data.size() == STELLAR_ADDRESS_SIZE;
    }

    /// Determines whether a string makes a valid  address.
    static bool isValid(const std::string& string);

    /// Initializes a Stellar address with a string representation.
    Address(const std::string& string);

    /// Initializes a Stellar address with a collection of bytes.
    Address(const std::vector<uint8_t>& data);

    /// Initializes a Stellar address with a public key.
    Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;

    /// CRC16 implementation compatible with the Stellar version
    /// Ported from this implementation: http://introcs.cs.princeton.edu/java/61data/CRC16CCITT.java.html
    /// Initial value changed to 0x0000 to match Stellar
    static uint16_t crc16(uint8_t *bytes, uint32_t length);
};

static inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

}} // namespace

/// Wrapper for C interface.
struct TWStellarAddress {
    TW::Stellar::Address impl;
};
