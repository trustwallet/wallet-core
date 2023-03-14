// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "Data.h"
#include "../uint256.h"

#include <cstdint>
#include <string>
#include <vector>

namespace TW::Ethereum {

/// Implementation of Ethereum's RLP encoding.
///
/// - SeeAlso: https://github.com/ethereum/wiki/wiki/RLP
struct RLP {
    /// Encodes a string;
    static Data encode(const std::string& string) noexcept {
        return encode(Data(string.begin(), string.end()));
    }

    static Data encode(uint8_t number) noexcept { return encode(uint256_t(number)); }

    static Data encode(uint16_t number) noexcept { return encode(uint256_t(number)); }

    static Data encode(int32_t number) noexcept {
        if (number < 0) {
            return {}; // RLP cannot encode negative numbers
        }
        return encode(static_cast<uint32_t>(number));
    }

    static Data encode(uint32_t number) noexcept { return encode(uint256_t(number)); }

    static Data encode(int64_t number) noexcept {
        if (number < 0) {
            return {}; // RLP cannot encode negative numbers
        }
        return encode(static_cast<uint64_t>(number));
    }

    static Data encode(uint64_t number) noexcept { return encode(uint256_t(number)); }

    static Data encode(const uint256_t& number) noexcept;

    /// Wraps encoded data as a list.
    static Data encodeList(const Data& encoded) noexcept;

    /// Encodes a block of data.
    static Data encode(const Data& data) noexcept;

    /// Encodes a static array.
    template <std::size_t N>
    static Data encode(const std::array<uint8_t, N>& data) noexcept {
        if (N == 1 && data[0] <= 0x7f) {
            // Fits in single byte, no header
            return Data(data.begin(), data.end());
        }

        auto encoded = encodeHeader(data.size(), 0x80, 0xb7);
        encoded.insert(encoded.end(), data.begin(), data.end());
        return encoded;
    }

    /// Encodes a list of elements.
    template <typename T>
    static Data encodeList(T elements) noexcept {
        auto encodedData = Data();
        for (const auto& el : elements) {
            auto encoded = encode(el);
            if (encoded.empty()) {
                return {};
            }
            encodedData.insert(encodedData.end(), encoded.begin(), encoded.end());
        }

        auto encoded = encodeHeader(encodedData.size(), 0xc0, 0xf7);
        encoded.insert(encoded.end(), encodedData.begin(), encodedData.end());
        return encoded;
    }

    /// Encodes a list header.
    static Data encodeHeader(uint64_t size, uint8_t smallTag, uint8_t largeTag) noexcept;

    struct DecodedItem {
        std::vector<Data> decoded;
        Data remainder;
    };

    static DecodedItem decodeList(const Data& input);
    /// Decodes data, remainder from RLP encoded data
    static DecodedItem decode(const Data& data);

    /// Returns the representation of an integer using the least number of bytes needed, between 1 and 8 bytes, big endian
    static Data putVarInt(uint64_t i) noexcept;
    /// Parses an integer of given size, between 1 and 8 bytes, big endian
    static uint64_t parseVarInt(size_t size, const Data& data, size_t index);
};

} // namespace TW::Ethereum
