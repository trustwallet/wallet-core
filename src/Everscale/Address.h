// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include <optional>
#include <string>

namespace TW::Everscale {

class Address {
public:
    /// Number of bytes in an address
    static const size_t size = Hash::sha256Size;

    /// Hex address length
    static const size_t hexAddrLen = size * 2;

    using MsgAddressInt = std::pair<int8_t, std::array<byte, Address::size>>;
    using MaybeWorkchainInfos = std::optional<std::pair<int8_t, uint32_t>>;

    std::int8_t _workchainId;
    std::array<byte, size> _address{};

    /// Determines whether a string makes a valid address.
    [[nodiscard]] static bool isValid(const std::string& string) noexcept;

    /// Initializes a Everscale address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes a Everscale address with a public key and workchain id.
    explicit Address(const PublicKey& publicKey, int8_t workchainId);

    /// Returns a string representation of the address.
    [[nodiscard]] std::string string() const;

private:
    static MaybeWorkchainInfos parseWorkchainId(const std::string& string);
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs._workchainId == rhs._workchainId && lhs._address == rhs._address;
}

} // namespace TW::Everscale
