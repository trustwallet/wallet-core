// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <array>
#include <memory>
#include <vector>

#include "Data.h"
#include "Hash.h"

namespace TW::CommonTON {

class Cell {
public:
    constexpr static uint16_t MAX_BITS = 1023;
    constexpr static uint8_t MAX_REFS = 4;

    using Ref = std::shared_ptr<Cell>;
    using Refs = std::array<Ref, MAX_REFS>;
    using CellHash = std::array<uint8_t, Hash::sha256Size>;

    bool finalized = false;
    uint16_t bitLen = 0;
    std::vector<uint8_t> data{};
    uint8_t refCount = 0;
    Refs references{};

    uint16_t depth = 0;
    CellHash hash{};

    Cell() = default;

    Cell(uint16_t bitLen, std::vector<uint8_t> data, uint8_t refCount, Refs references)
        : bitLen(bitLen), data(std::move(data)), refCount(refCount), references(std::move(references)) {}

    // Deserialize from Base64
    static std::shared_ptr<Cell> fromBase64(const std::string& encoded);

    // Deserialize from BOC representation
    static std::shared_ptr<Cell> deserialize(const uint8_t* _Nonnull data, size_t len);

    // Serialize to binary stream
    void serialize(Data& os) const;

    // Compute cell depth and hash
    void finalize();

    [[nodiscard]] inline std::pair<uint8_t, uint8_t> getDescriptorBytes() const noexcept {
        const uint8_t d1 = refCount;
        const uint8_t d2 = (static_cast<uint8_t>(bitLen >> 2) & 0b11111110) | (bitLen % 8 != 0);
        return std::pair<uint8_t, uint8_t>{d1, d2};
    }

    [[nodiscard]] inline size_t serializedSize(uint8_t refSize) const noexcept {
        return 2 + (bitLen + 7) / 8 + refCount * refSize;
    }
};

} // namespace TW::CommonTON
