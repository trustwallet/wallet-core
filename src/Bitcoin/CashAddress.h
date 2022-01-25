// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../PublicKey.h"
#include "../Data.h"

#include <cstdint>
#include <string>

namespace TW::Bitcoin {

class CashAddress {
  public:
    /// Number of bytes in an address.
    static const size_t size = 34;

    /// Address data consisting of a prefix byte followed by the public key
    /// hash.
    std::array<byte, size> bytes;

    /// Cash address human-readable part
    const std::string hrp;

    /// Determines whether a collection of bytes makes a valid  address.
    template <typename T>
    static bool isValid(const T& data) {
        return data.size() == size && (data[0] == 0 || data[0] == 1);
    }

    /// Determines whether a string makes a valid  address.
    static bool isValid(const std::string& hrp, const std::string& string);

    /// Initializes a  address with a string representation.
    explicit CashAddress(const std::string& hrp, const std::string& string);

    /// Initializes a  address with a collection of bytes.
    explicit CashAddress(const std::string& hrp, const Data& data);

    /// Initializes a  address with a public key.
    explicit CashAddress(const std::string& hrp, const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;

    /// Returns the legacy address representation.
    Address legacyAddress() const;
};

inline bool operator==(const CashAddress& lhs, const CashAddress& rhs) {
    return lhs.bytes == rhs.bytes;
}

class BitcoinCashAddress : public CashAddress {
  public:
    static const std::string hrp; // HRP_BITCOINCASH

    /// Initializes an address with a string representation.
    explicit BitcoinCashAddress(const std::string& string)
        : CashAddress(hrp, string) {}

    /// Initializes an address with a collection of bytes.
    explicit BitcoinCashAddress(const Data& data)
        : CashAddress(hrp, data) {}

    /// Initializes an address with a public key.
    explicit BitcoinCashAddress(const PublicKey& publicKey)
        : CashAddress(hrp, publicKey) {}

    static bool isValid(const std::string& string) {
        return CashAddress::isValid(hrp, string);
    }
};

class ECashAddress : public CashAddress {
  public:
    static const std::string hrp; // HRP_ECASH

    /// Initializes an address with a string representation.
    explicit ECashAddress(const std::string& string)
        : CashAddress(hrp, string) {}

    /// Initializes an address with a collection of bytes.
    explicit ECashAddress(const Data& data)
        : CashAddress(hrp, data) {}

    /// Initializes an address with a public key.
    explicit ECashAddress(const PublicKey& publicKey)
        : CashAddress(hrp, publicKey) {}

    static bool isValid(const std::string& string) {
        return CashAddress::isValid(hrp, string);
    }
};

} // namespace TW::Bitcoin
