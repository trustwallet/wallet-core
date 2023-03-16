// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>
#include <optional>
#include <array>

#include "Hash.h"

namespace TW::CommonTON {

struct AddressData {
    /// Number of bytes in an address
    static const size_t size = Hash::sha256Size;

    /// Hex address length
    static const size_t hexAddrLen = size * 2;

    /// Workchain ID (-1 for masterchain, 0 for base workchain)
    std::int8_t workchainId{};

    /// StateInit hash
    std::array<byte, AddressData::size> hash{};

    explicit AddressData(int8_t workchainId, std::array<byte, size> hash)
        : workchainId(workchainId), hash(hash) {}

    explicit AddressData() = default;
};

struct RawAddress {
    static bool isValid(const std::string& string);
    static AddressData splitAddress(const std::string& address);
    static std::string to_string(const AddressData& addressData);
};

} // namespace TW::CommonTON
