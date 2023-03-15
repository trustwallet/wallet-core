// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "Data.h"
#include "../PublicKey.h"

#include <TrustWalletCore/TWHRP.h>

#include <cstdint>
#include <string>

namespace TW::Bitcoin {

inline const std::string gBitcoinCashHrp{HRP_BITCOINCASH};
inline const std::string gECashHrp{HRP_ECASH};

class CashAddress {
public:
    /// Number of bytes in an address.
    static constexpr size_t size{34};

    /// Address data consisting of a prefix byte followed by the public key
    /// hash.
    std::array<byte, size> bytes{};

    /// Cash address human-readable part
    const std::string hrp;

    /// Determines whether a collection of bytes makes a valid  address.
    template <typename T>
    static bool isValid(const T& data) {
        return data.size() == size && (data[0] == 0 || data[0] == 1);
    }

    /// Determines whether a string makes a valid  address.
    static bool isValid(const std::string& hrp, const std::string& string) noexcept;

    /// Initializes a  address with a string representation.
    explicit CashAddress(const std::string& hrp, const std::string& string);

    /// Initializes a  address with a public key.
    explicit CashAddress(std::string hrp, const PublicKey& publicKey);

    /// Returns a string representation of the address.
    [[nodiscard]] std::string string() const noexcept;

    /// Returns the legacy address representation.
    [[nodiscard]] Address legacyAddress() const noexcept;

    Data getData() const;
};

class BitcoinCashAddress : public CashAddress {
public:
    /// Initializes an address with a string representation.
    explicit BitcoinCashAddress(const std::string& string) : CashAddress(gBitcoinCashHrp, string) {}

    /// Initializes an address with a public key.
    explicit BitcoinCashAddress(const PublicKey& publicKey) : CashAddress(gBitcoinCashHrp, publicKey) {}

    static bool isValid(const std::string& string) noexcept {
        return CashAddress::isValid(gBitcoinCashHrp, string);
    }
};

class ECashAddress : public CashAddress {
public:
    /// Initializes an address with a string representation.
    explicit ECashAddress(const std::string& string) : CashAddress(gECashHrp, string) {}

    /// Initializes an address with a public key.
    explicit ECashAddress(const PublicKey& publicKey) : CashAddress(gECashHrp, publicKey) {}

    static bool isValid(const std::string& string) noexcept {
        return CashAddress::isValid(gECashHrp, string);
    }
};  

} // namespace TW::Bitcoin
